//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "../trafficsrcs/TrafficSource.h"

#include "../../../common/GlobalMacros.h"
#include "../../../common/ValidationHandler.h"


TrafficSource::TrafficSource()
{
    // Initialize the module variables
    interBurstMsg = nullptr;
    intraBurstMsg = nullptr;
    pendingBursts = 0;
    pendingIntraBurstSize = 0;
}

TrafficSource::~TrafficSource()
{
    // Clean the self messages
    cancelAndDelete(intraBurstMsg);
    cancelAndDelete(interBurstMsg);
}


void TrafficSource::initialize()
{

    // Load the global stream ID of the generated stream
    globalStreamId = getParentModule()->par("globalStreamId");

    // Load startup parameters
    generatorEnable = getParentModule()->par("isEnabled").boolValue();
    simtime_t offset = getParentModule()->par("offsetDuration");
    bool usingOffset = getParentModule()->par("enableOffset").boolValue();

    // Initialize the self-messages
    interBurstMsg = new cMessage("interBurstMsg", 0);
    intraBurstMsg = new cMessage("intraBurstMsg", 0);

    _burst_src_verbose_
    {
        EV << "[Traffic Source] global stream ID: \t" << globalStreamId << "\n";
        EV << "[Traffic Source] is enabled: \t" << generatorEnable << "\n";
        EV << "[Traffic Source] offset enabled: \t" << usingOffset << "\n";
        EV << "[Traffic Source] offset duration: \t" << offset << "\n";
    }

    // Start the generator if enabled
    if (generatorEnable)
    {
        // Initialize generation message
        if (usingOffset)
        {
            // If enabled start the burst generator with an offset
            scheduleNextBurst(offset);
        }
        else
        {
            // Start burst generator without an offset
            scheduleNextBurst(0);
        }
    }
}



void TrafficSource::handleMessage(cMessage *msg)
{

    // Check for incoming generator messages
    if (msg->isSelfMessage())
    {
        // Check for next burst element to send
        if (msg == intraBurstMsg)
        {
            // A packet within a burst is being generated now.
            sendPacket();
        }
        // Check for new burst arrival
        else if (msg == interBurstMsg)
        {

            // Add one pending burst
            pendingBursts++;

            // Warning if too many burst are pending (Indicator for a wrong burst activation model)
            if (pendingBursts > 1000)
            {
                std::stringstream ss;
                ss << "Pending bursts: " << pendingBursts << " at simTime: " << simTime() << "\n";
                std::string s = ss.str();
                ValidationHandler::ThrowError(s.data());
            }

            // Schedule the next burst activation
            scheduleNextBurst(0);

            // Check if the next burst is ready to start
            scheduleNextIntraBurst();
        }
        // There should not be any other message types
        else
        {
            ValidationHandler::ThrowError("ERROR: Receiving unexpected message!");
        }
    }
    // There should only be self-messages scheduled in this module
    else
    {
        ValidationHandler::ThrowError("ERROR: Receiving unexpected message!");
    }
}





void TrafficSource::scheduleNextBurst(simtime_t offset)
{

    // Get the next time to schedule the burst based on the period (volatile parameter)
    simtime_t period = getParentModule()->par("periodBase");

    // Bound the period towards the minimum and maximum
    period = truncate_min_max<simtime_t>(period, getParentModule()->par("periodMin"),
            getParentModule()->par("periodMax"));

    // Schedule the next burst
    EV << "Next burst will be generated in " << period << "s.\n";

    // Check sanity
    if (simTime() + period < simTime())
    {
        ValidationHandler::ThrowError("ERROR: Period is <= 0s!");
    }

    // Schedule the next burst
    scheduleAt(simTime() + period + offset, interBurstMsg);
}



void TrafficSource::scheduleNextIntraBurst()
{

    // Directly return if a burst is already active
    if (intraBurstMsg->isScheduled() || pendingIntraBurstSize > 0)
    {
       return;
    }

    pendingBursts--;

    pendingIntraBurstSize = int(getParentModule()->par("burstSizeBase").doubleValue());
    pendingIntraBurstSize = truncate_min_max<int>(pendingIntraBurstSize, getParentModule()->par("burstSizeMin").intValue(),
            getParentModule()->par("burstSizeMax").intValue());
    if (pendingIntraBurstSize < 1)
    {
        ValidationHandler::ThrowError("ERROR: Generating a burst with zero packets!");
    }

    // Start the burst "now"
    scheduleAt(simTime(), intraBurstMsg);

}



void TrafficSource::sendPacket()
{

    // Check if the burst is over
    if (pendingIntraBurstSize == 0)
    {
        // If there are pending bursts, start a new burst
        if (pendingBursts > 0)
        {
            // Calculate the length of the next burst
            pendingBursts--;
            pendingIntraBurstSize = int(getParentModule()->par("burstSizeBase").doubleValue());
            pendingIntraBurstSize = truncate_min_max<int>(pendingIntraBurstSize, getParentModule()->par("burstSizeMin").intValue(),
                    getParentModule()->par("burstSizeMax").intValue());
            if (pendingIntraBurstSize < 1)
            {
                ValidationHandler::ThrowError("ERROR: Generating a burst with zero packets!");
            }
        }
    }


    // Create and send a new message if there are pending packages to send
    if(!intraBurstMsg->isScheduled()){

        // Check if the burst is not over yet
        if (pendingIntraBurstSize > 0)
        {
            // Send out a message
            cMessage *genMsg = new cMessage();
            send(genMsg, "messageGenerationOut");

            // Decrease the burst counter
            pendingIntraBurstSize--;

            //Always schedule the intra burst self message after a sending event to block further sending attempts for the inter delay time
            simtime_t intraBurstPeriod = truncate_min_max<simtime_t>(getParentModule()->par("intraBurstDurationBase"),
                    getParentModule()->par("intraBurstDurationMin"), getParentModule()->par("intraBurstDurationMax"));

            // Sanity Check
            if(intraBurstPeriod == 0 && (pendingIntraBurstSize > 0 || pendingBursts > 0)){
                ValidationHandler::ThrowError("ERROR: intraBurstPeriod is 0s for burst sizes > 1!");
            }

            // Sanity Check
            if(intraBurstMsg->isScheduled()){
                ValidationHandler::ThrowError("ERROR: intraBurstMsg is aready scheduled!");
            }

            // Always trigger the next intra burst event.
            // If this is the last activation of the current burst, it will directly trigger the next pending burst.
            scheduleAt(simTime() + intraBurstPeriod, intraBurstMsg);
        }




    }

}




















