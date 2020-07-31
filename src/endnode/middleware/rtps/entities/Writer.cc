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

#include "../../../../endnode/middleware/rtps/entities/Writer.h"

#include <math.h>
#include "./../messages/RTPSEthMessage_m.h"
#include "./../../../../common/ValidationHandler.h"
#include "./../../../../endnode/middleware/rtps/messages/RegisterEntity_m.h"
#include "./../../../../endnode/middleware/rtps/messages/Sample_m.h"
#include "../../../../common/RtpsMacros.h"



Writer::Writer()
{
    currentSampleSequenceNumber = 0;
}

Writer::~Writer()
{
    cancelAndDelete(sendEvent);
    cancelAndDelete(registerEvent);
}



void Writer::initialize()
{

    // Create internal self-messages
    sendEvent = new cMessage("event");
    registerEvent = new cMessage("register_event");

    // Load the parameters
    sourceMac = par("sourceMac");
    destinationMac = par("destinationMac");
    entityId = par("entityId");
    destinationEntityId = par("destinationEntityId");
    streamEthernetPriority = par("streamEthernetPriority");
    globalStreamId = par("globalStreamId");
    shapingDuration = par("shapingDuration");


    // Set other QoS parameters
    historySizeQoS = par("historySizeQoS");
    lifespan = par("lifespan");
    bestEffort = par("bestEffort");
    fragmentSize = int( par("fragmentSize"));
    transportPriority = par("transportPriority");

    // Check the maximum fragment size
    if(fragmentSize > _RTPS_MAX_FRAG_SIZE_)
    {
        ValidationHandler::ThrowError("Fragment size too high!");
    }

    // Register the reader at the participant (message dispatcher)
    scheduleAt(simTime(),registerEvent);

}

void Writer::finish()
{
}

void Writer::registerWriter()
{
    // Each writer sends an initialize message to the corresponding dispatcher!
    RegisterEntity* registerMessage = new RegisterEntity();
    registerMessage->setReader(false);
    registerMessage->setWriter(true);
    registerMessage->setEntityId(entityId);
    send(registerMessage, "dispatcherOut");
}

void Writer::handleMessage(cMessage *msg)
{

    if (dynamic_cast<Sample*>(msg)!=NULL){

        // First create a new history element related to the sample and push it to the history
        HistorySampleEntry* h_entry = new HistorySampleEntry();
        historyCache.push_back(h_entry);

        // Get relevant informations from the generator
        Sample *sample = check_and_cast<Sample*>(msg);
        h_entry->sampleSize = sample->getSize();
        h_entry->globalStreamId  =  globalStreamId;
        h_entry->generationTime = sample->getGenerationTime();

        if(sample->getGlobalStreamId() != this->globalStreamId){
            ValidationHandler::ThrowError("WARNING: Global Stream IDs from writer and generator do not match!");
        }

        // Note the sample writing time
        h_entry->writerArrivalTime = simTime();
        h_entry->writerFirstInjectionTime = 0.0;

        // Assign and increment the Sequence Number
        h_entry->sampleSequenceNumber = currentSampleSequenceNumber;
        currentSampleSequenceNumber++;

        // Calculate the number of fragments
        h_entry->numberFragments = int(ceil(double(h_entry->sampleSize)/double(fragmentSize)));

        // Initialize the fragment array
        h_entry->sampleFragmentArray = new SampleFragment*[h_entry->numberFragments];

        for(int i = 0; i < h_entry->numberFragments; i++)
        {
            // Create the Fragment
            h_entry->sampleFragmentArray[i] = new SampleFragment();

            //Insert and link the fragment
            SampleFragment* current_fragment = h_entry->sampleFragmentArray[i];
            current_fragment->sampleReference = h_entry;

            // Set fragment sequence number and size
            current_fragment->fragmentSequenceNumber = i;
            current_fragment->fragmentSize = std::min( int(fragmentSize) ,int(h_entry->sampleSize - (i*fragmentSize)));

        }

        // Check the size of the history cache, delete last sample if the maximum cache size has been reached
        if(historyCache.size() > historySizeQoS)
        {

            // Get the sample to delete
            HistorySampleEntry* to_delete = historyCache.front();

            // Remove it from the list
            historyCache.pop_front();

            // Delete all the fragments here! Best in the destructor!
            delete to_delete;
        }

        /* Check if there is something new to send */
        sendMessage();

        /* Finally delete the received message */
        delete msg;

    } else if(dynamic_cast<RTPSEthMessage*>(msg)!=NULL)
    {

        RTPSEthMessage *rtps_msg = check_and_cast<RTPSEthMessage*>(msg);

        // Check for ACK message and evaluate
//        if(rtps_msg->getNackFragIsSet())
//        {
//            if(!bestEffort){
//            } else {
//            }
//        }

        // Check if there is something to send
        sendMessage();

        //Finally delete the message
        delete msg;
        return;

    } else if(msg == sendEvent)
    {
        sendMessage();
    } else if(msg == registerEvent)
    {
        registerWriter();
        return;
     } else
     {
        ValidationHandler::ThrowError("WARNING: Middleware received unsupported Message type!");
    }


}



bool Writer::addNextFragmentToSendList()
{

    // Take the first unsent entry from the sample with the lowest sample sequence number for now!
    for(HistorySampleEntry* current_he: historyCache)
    {

        for(int sf = 0; sf < current_he->numberFragments; sf++)
        {

            SampleFragment* current_sf = current_he->sampleFragmentArray[sf];
            if(current_sf->acked)
            {
                continue;
            }

            if(!current_sf->send)
            {
                send_queue.push_back(current_sf);
                return true;

            }
        }
    }
    return false;
}

// Check if the lifespan of a sample has been exeeded
void Writer::checkLifespanOfSamples()
{

    // Check for inifinite lifespan (==0)
    if(lifespan == 0)
    {
        return;
    }

    if(historyCache.size() == 0 )
    {
        return;
    }

    while(1)
    {

        HistorySampleEntry* current_he = historyCache.front();

        if(current_he == nullptr)
        {
            return;
        }

        // Check the lifespan condition
        if(simTime() - current_he->writerArrivalTime > lifespan)
        {

            historyCache.pop_front();
            delete current_he;

        // Since the current lifespan is ordered, we can break here
        } else
        {
            break;
        }

    }
}



void Writer::sendMessage()
{

    // Check if the writer is currently allowed to send
    if(sendEvent->isScheduled())
    {
        return;
    }

    // Check the lifespan before sending
    checkLifespanOfSamples();

    // Choose the next fragment
    if(send_queue.empty()){
        if(addNextFragmentToSendList() == false)
        {
            return;
        }
    }

    // Prepare for sending. Get the next fragment from the list
    SampleFragment* sf = send_queue.front();
    send_queue.pop_front();
    RTPSEthMessage* rtpsMessage = createRtpsMsgFromFragment(sf);
    rtpsMessage->setRtpsMsgSequenceId(getNextUniqueRtpsMsgId());

    // Send the message out to the participant
    send(rtpsMessage , gate("dispatcherOut"));

    // Mark the current fragment as sended
    sf->sendTime = simTime();
    sf->send = true;


    // Schedule the next event with ideal shaping
    simtime_t time_to_send = simTime() + shapingDuration;
    scheduleAt(time_to_send, sendEvent);
}



RTPSEthMessage* Writer::createRtpsMsgFromFragment(SampleFragment* sampleFragment)
{

    // Create a new RTPS message instance
    RTPSEthMessage* rtpsMessage = new RTPSEthMessage();

    // General parameters
    rtpsMessage->setGlobalStreamId(sampleFragment->sampleReference->globalStreamId);

    // Routing parameters (Ethernet)
    rtpsMessage->setDestinationMac(par("destinationMac"));
    rtpsMessage->setSourceMac(par("sourceMac"));
    rtpsMessage->setEthernetPriority(par("ethernetPriority"));

    // Insert the RTPS Header
    rtpsMessage->setSourceParticipantId(getParentModule()->par("participantId"));

    // Insert InfoDestination submessage
    rtpsMessage->setInfoDestinationIsSet(true);
    rtpsMessage->setDestinationMac(par("destinationMac"));

    // Insert InfoTimestamp
    if(sampleFragment->fragmentSequenceNumber == 0)
    {
        rtpsMessage->setInfoTimestampIsSet(true);
        rtpsMessage->setTimestamp(sampleFragment->sampleReference->writerArrivalTime);

    }

    // Insert DataFrag
    rtpsMessage->setDataFragIsSet(true);
    rtpsMessage->setWriterId(entityId);
    rtpsMessage->setReaderId(destinationEntityId);
    rtpsMessage->setFragmentStartingNum(sampleFragment->fragmentSequenceNumber);
    rtpsMessage->setFragmentsInSubmessage(1);
    rtpsMessage->setFragmentSize(sampleFragment->fragmentSize);
    rtpsMessage->setWriterSN(sampleFragment->sampleReference->sampleSequenceNumber);
    rtpsMessage->setSampleSize(sampleFragment->sampleReference->sampleSize);

    rtpsMessage->setTransportPriority(transportPriority);

    if(!bestEffort)
    {
        // Add Heartbeat submessage pattern here
    }

    // Other
    rtpsMessage->setGeneralFragmentSize(fragmentSize);

    // Statistics
    // The first fragment send regarding a sample defines the sample injection time
    if(sampleFragment->sampleReference->writerFirstInjectionTime.dbl() == 0.0)
    {
        sampleFragment->sampleReference->writerFirstInjectionTime = simTime();
    }
    rtpsMessage->setGenerationTime(sampleFragment->sampleReference->generationTime);
    rtpsMessage->setWriterArrivalTime(sampleFragment->sampleReference->writerArrivalTime);
    rtpsMessage->setWriterFirstInjectionTime(sampleFragment->sampleReference->writerFirstInjectionTime);

    // Last Calculate the message size of the complete RTPS message
    calculateRtpsMsgSize(rtpsMessage);

    // Return the message
    return rtpsMessage;
}
