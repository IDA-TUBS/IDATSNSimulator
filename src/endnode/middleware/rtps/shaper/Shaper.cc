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

#include "../../../../endnode/middleware/rtps/shaper/Shaper.h"

#include "../../../../common/ValidationHandler.h"

Define_Module(Shaper);

Shaper::~Shaper()
{
    cancelAndDelete(nextMsgReady);
}


void Shaper::initialize()
{
    // Get parameter to enable or disable the shaper
    shaperEnabled = par("shaperEnabled");

    // Get the shaping distance
    shapingDuration = par("shapingDuration");

    // Self messages to control the shaping
    nextMsgReady = new cMessage("nextMsgReady");

}

void Shaper::handleMessage(cMessage *msg)
{
    // Check if enabled
    if(shaperEnabled){

        // Check message type
        if(dynamic_cast<RTPSEthMessage*>(msg)!=NULL){

            // Add the message to the queue
            RTPSEthMessage *rtps_msg = check_and_cast<RTPSEthMessage*>(msg);
            rtpsMsgQueue.push_back(rtps_msg);

            // Try to send the message directly
            send_from_queue();

        } else if(msg == nextMsgReady){
            // Send the next message
            send_from_queue();
        } else {
            ValidationHandler::ThrowError("ERROR: Invalid message type to be shaped!");
        }

    } else {
        // If shaper is disabled, directly pass the message to the output
        send(msg,"adapterOut");
    }

}


void Shaper::send_from_queue()
{

    // Return if the shaping duration has not expired yet so that nextMsgReady is still scheduled
    if(nextMsgReady->isScheduled()){
        return;
    } else {
        // Send out the next message
        RTPSEthMessage* rtps_msg = rtpsMsgQueue.front();

        // Return if the queue is empty, the next message can be send immediately
        if(rtps_msg == nullptr){
            return;
        }

        // Remove the message from the queue
        rtpsMsgQueue.pop_front();
        // Initialize the shaping duration
        scheduleAt(simTime() +  shapingDuration, nextMsgReady);
        // Send out the message
        send(rtps_msg, "adapterOut");

    }
}


void Shaper::finish()
{
}





