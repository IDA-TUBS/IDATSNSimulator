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

#include "../../../../endnode/middleware/rtps/entities/Reader.h"

#include <string.h>
#include <math.h>
#include "./../messages/RTPSEthMessage_m.h"
#include "./../../../../common/EthernetMacros.h"
#include "./../../../../messagetype/EthernetFrame_m.h"
#include "./../../../../endnode/middleware/rtps/messages/RegisterEntity_m.h"
#include "./../../../../endnode/middleware/rtps/messages/Sample_m.h"



Reader::~Reader()
{
    cancelAndDelete(registerEvent);
}



void Reader::initialize()
{

    // Get the configuration parameters
    // Best effort vs. reliable
    bestEffort = par("bestEffort");
    // Static assignment if discovery is disabled
    entityId = par("entityId");
    // Mac Adress of the egress port
    sourceMac = par("sourceMac");

    // History QoS
    historySizeQoS = par("historySizeQoS");
    // Lifespan QoS
    lifespan = par("lifespan");
    // Latency Budget QoS
    deadline = par("deadline");
    // Initialize the cache
    readerHistoryCache = new ReaderHistory(historySizeQoS);

    for(int i = 0; i < readerHistoryCache->maxNbrStreams; i++)
    {
        readerHistoryCache->cache[i] = nullptr;
    }

    // Register the reader at the participant (message dispatcher)
    registerEvent = new cMessage("registerEvent");
    scheduleAt(simTime(),registerEvent);

    // Statistics
    SampleLatencySinceGenerationStat.setName("Latency Since Generation");
    SampleLatencySinceGenerationStatVec.setName("Latency Since Generation Vector");
    SampleLatencySinceWriterArrivalStat.setName("Latency Since Sample Arrival at Writer");
    SampleLatencySinceWriterArrivalStatVec.setName("Latency Since Sample Arrival at Writer Vector");
    SampleLatencySinceSampleInjectionStat.setName("Latency since first Sample Injection at Writer");
    SampleLatencySinceSampleInjectionStatVec.setName("Latency since first Sample Injection at Writer Vector");
    frameLatencyInjectionToReception.setName("Network time of individual RTPS messages");
    frameLatencyInjectionToReceptionVec.setName("Network time of individual RTPS messages Vector");

}

void Reader::registerReader(){
    // Each reader sends an initialize message to the corresponding dispatcher!
    RegisterEntity* registerMessage = new RegisterEntity();
    registerMessage->setReader(true);
    registerMessage->setWriter(false);
    registerMessage->setEntityId(entityId);
    send(registerMessage, "dispatcherOut");
}

void Reader::finish()
{
    // Record the statistsics
    SampleLatencySinceGenerationStat.recordAs("SampleLatencySinceGenerationStat");
    SampleLatencySinceWriterArrivalStat.recordAs("SampleLatencySinceWriterArrivalStat");
    SampleLatencySinceSampleInjectionStat.recordAs("SampleLatencySinceSampleInjectionStat");
    frameLatencyInjectionToReception.recordAs("frameLatencyInjectionToReception");
}




void Reader::handleMessage(cMessage *msg)
{
    // XXX Check if wrong destination mac!

    // Check message type
    if (dynamic_cast<RTPSEthMessage*>(msg)!=NULL){

        RTPSEthMessage *rtpsMsg = check_and_cast<RTPSEthMessage*>(msg);

        // Do statistical evaluation
        simtime_t timeSinceInjection= simTime() - rtpsMsg->getInjectionTime();
        frameLatencyInjectionToReception.collect(timeSinceInjection.dbl());
        frameLatencyInjectionToReceptionVec.record(timeSinceInjection.dbl());

        // Get the corresponding history cache line
        std::list<HistorySampleEntry*>*  relevantCacheLine = getCache(rtpsMsg->getSourceParticipantId(), rtpsMsg->getWriterId());

        // Add the incoming message to the history cache
        updateCache(rtpsMsg, relevantCacheLine);

        // Check the current state of the history
        checkLifespan(relevantCacheLine);
        cleanHistoryCache(relevantCacheLine);

        // After each incoming message check for completion
        checkCompletion(relevantCacheLine);

    } else if(msg == registerEvent){
        registerReader();
        return;
    } else {
        ValidationHandler::ThrowError("Invalid message type received at reader.");
    }

    delete msg;
}





bool Reader::relevantNewSample(int sequenceNumber, int writerEntityId, int sourceParticipantId,  std::list<HistorySampleEntry*>* historyCache){

    int highestSN = -1;

    if(historyCache == nullptr){
        ValidationHandler::ThrowError("ERROR!!! nullptr");
    }


    for(HistorySampleEntry* current: *historyCache){

        // First filter for relevant samples
        // Therefore identify the relevant unique writer via the participant ID and its writers entity ID
        if(current->writerParticipantEntityId != sourceParticipantId)
        {
            continue;
        }

        if(current->writerEntityId != writerEntityId){
            continue;
        }

        if(current->historySizeExeeded){
            continue;
        }

        // Then get the information from the relevant stored samples
        if(current->sampleSequenceNumber == sequenceNumber){
            // Sample is already in history cache
            return false;
        }

        if(current->sampleSequenceNumber > highestSN){
            // Get the highest current SN of the corresponding writer
            highestSN = current->sampleSequenceNumber;
        }

    }

    if(bestEffort){
        if(sequenceNumber > highestSN){
            return true;
        } else {
            return false;
        }
    }

    if(sequenceNumber > highestSN - historySizeQoS + 1){
        return true;
    } else {
        return false;
    }

    return true;
}


std::list<HistorySampleEntry*>*  Reader::getCache(int sourceParticipantId, int writerId){

    for(int i = 0; i < readerHistoryCache->maxNbrStreams; i++)
    {
        if(readerHistoryCache->cache[i] == nullptr){
            break;
        }

        std::list<HistorySampleEntry*>* current = readerHistoryCache->cache[i];

        if(current->empty()){
            ValidationHandler::ThrowError("Empty initialized Cache detected in reader!");
        }

        HistorySampleEntry* entry = current->front();

        if(entry->writerParticipantEntityId == sourceParticipantId && entry->writerEntityId == writerId){
            return current;
        }
    }

    if(readerHistoryCache->nbrStreams > readerHistoryCache->maxNbrStreams){
        ValidationHandler::ThrowError("Two many streams for resources of reader");
    }

    readerHistoryCache->cache[readerHistoryCache->nbrStreams] = new std::list<HistorySampleEntry*>;
    readerHistoryCache->nbrStreams++;
    return readerHistoryCache->cache[readerHistoryCache->nbrStreams-1];

}


void Reader::updateCache(RTPSEthMessage* rtpsMsg, std::list<HistorySampleEntry*>* historyCache){

    // Debug
    EV << "Received fragment with SN: " << rtpsMsg->getWriterSN() << " and fragSN " << rtpsMsg->getFragmentStartingNum() << "\n";
//    EV << "... and participant ID: " << rtpsMsg->getSourceParticipantId() << " and WriterEnity ID:" << rtpsMsg->getWriterId() << "\n";
    // Check if any fragment of the corresponding sample has not been received yet
    int sampleSn = rtpsMsg->getWriterSN();

    // Store the relevant history entry
    HistorySampleEntry* returnEntry = nullptr;

    // Check if sample is already in the cache
    if(relevantNewSample(sampleSn, rtpsMsg->getWriterId(), rtpsMsg->getSourceParticipantId(), historyCache)){

        // Create the new entry
        HistorySampleEntry* newHistorySampleEntry= new HistorySampleEntry();

        // Push the new entry to the history cache
        historyCache->push_back(newHistorySampleEntry);

        // Assign the data to the newly created entry
        newHistorySampleEntry->sampleSize = rtpsMsg->getSampleSize();
        newHistorySampleEntry->sampleSequenceNumber = sampleSn;
        newHistorySampleEntry->globalStreamId = rtpsMsg->getGlobalStreamId();

        newHistorySampleEntry->generationTime = rtpsMsg->getGenerationTime();
        newHistorySampleEntry->writerArrivalTime = rtpsMsg->getWriterArrivalTime();
        newHistorySampleEntry->writerFirstInjectionTime = rtpsMsg->getWriterFirstInjectionTime();
        newHistorySampleEntry->subscriberReceiveTime = 0;

        newHistorySampleEntry->writerParticipantEntityId = rtpsMsg->getSourceParticipantId();
        newHistorySampleEntry->writerEntityId = rtpsMsg->getWriterId();
        newHistorySampleEntry->readerParticipantEntityId = getParentModule()->par("participantId");
        newHistorySampleEntry->readerEntityId = this->entityId;

        // Calculate and initialize the number of fragments
        newHistorySampleEntry->numberFragments = int(ceil(double(newHistorySampleEntry->sampleSize)/double(rtpsMsg->getGeneralFragmentSize())));

        newHistorySampleEntry->sampleFragmentArray = new SampleFragment*[newHistorySampleEntry->numberFragments];
        for(int i = 0; i < newHistorySampleEntry->numberFragments; i++){
            newHistorySampleEntry->sampleFragmentArray[i] = new SampleFragment();
            SampleFragment* currentFragment = newHistorySampleEntry->sampleFragmentArray[i];
            currentFragment->sampleReference = newHistorySampleEntry;
            currentFragment->fragmentSequenceNumber = i;
        }

    }



    /* Mark the fragment as received */
    int fragmentSequenceNumber = rtpsMsg->getFragmentStartingNum();

    for(HistorySampleEntry* current: *historyCache){
        if(rtpsMsg->getSourceParticipantId() != current->writerParticipantEntityId){
            continue;
        }

        if(rtpsMsg->getWriterId() != current->writerEntityId){
            continue;
        }

        if(sampleSn != current->sampleSequenceNumber){
            continue;
        }

        // Now find the relevant fragment
        for(int sf = 0; sf < current->numberFragments; sf++){
            SampleFragment* currentFragment = current->sampleFragmentArray[sf];
            if(currentFragment->fragmentSequenceNumber - rtpsMsg->getFragmentStartingNum() + 1 <= rtpsMsg->getFragmentsInSubmessage()){
                currentFragment->received = true;
            }
        }
    }

}


void Reader::checkLifespan(std::list<HistorySampleEntry*>* historyCache){

    for(HistorySampleEntry* current: *historyCache){
        if(current->lifespanExpired){
            continue;
        }
        if(current->writerArrivalTime + lifespan < simTime()){
            current->lifespanExpired = true;
        }

    }
}

void Reader::cleanHistoryCache(std::list<HistorySampleEntry*>* historyCache){


    if(historyCache->size() > this->historySizeQoS){
        HistorySampleEntry* to_delete = historyCache->front();
        historyCache->pop_front();
        delete to_delete;
    }

    // Check the history QoS parameter for the writer corresponding to the received sample
//    for(HistorySampleEntry* current: *historyCache){
//        // To be implemented
//    }

}


void Reader::insertSampleInCache(HistorySampleEntry* sample){
    // TBD
}



void Reader::checkCompletion(std::list<HistorySampleEntry*>* historyCache){

    // Run through all samples and check for completion
    for(HistorySampleEntry* current: *historyCache){

        // Check if already completed or not relevant any more
        if(current->complete || current->lifespanExpired || current->historySizeExeeded){
            continue;
        }

        bool complete = true;
        for(int frag = 0; frag < current->numberFragments;frag++){
            SampleFragment* current_frag = current->sampleFragmentArray[frag];
            if(!current_frag->received){
                complete = false;
                break;
            }

        }

        // Do statistical evaluation
        if(complete){

            // Mark the sample as complete
            current->complete = true;

            // First insert the current timestamp
            current->subscriberReceiveTime = simTime();
            // Then send the sample to the sink
            sendSampleToSink(current);


            // Do statistical evaluation
            // From generation to complete reception
            simtime_t latencyGeneration = simTime() - current->generationTime;
            double latencyGenerationDouble = latencyGeneration.dbl();
            SampleLatencySinceGenerationStat.collect(latencyGenerationDouble);
            SampleLatencySinceGenerationStatVec.record(latencyGenerationDouble);
            // From arrival at writer to complete reception at reader
            simtime_t latencySinceWriterArrival = simTime() - current->writerArrivalTime;
            double latencySinceWriterArrivalDouble = latencySinceWriterArrival.dbl();
            SampleLatencySinceWriterArrivalStat.collect(latencySinceWriterArrivalDouble);
            SampleLatencySinceWriterArrivalStatVec.record(latencySinceWriterArrivalDouble);
            // From the injection of the first related RTPS message into the network to the complete reception at the reader
            simtime_t latencySinceFirstInjection = simTime() - current->writerFirstInjectionTime;
            double latencySinceFirstInjectionDouble = latencySinceFirstInjection.dbl();
            SampleLatencySinceSampleInjectionStat.collect(latencySinceFirstInjectionDouble);
            SampleLatencySinceSampleInjectionStatVec.record(latencySinceFirstInjectionDouble);

            // Evaluate the deadline violation
            if(latencySinceWriterArrivalDouble > deadline.dbl()){
                current->deadlineViolation = true;
            } else {
                current->deadlineViolation = false;
            }

            EV << "======================== SAMPLE FINISHED =====================\n";

        }

    }
}



void Reader::sendSampleToSink(HistorySampleEntry* entry){

    // Create and send the output message to the join_readers component
    Sample* sample = new Sample();

    //General Information
    sample->setSize(entry->sampleSize);
    sample->setSequenceNumber(entry->sampleSequenceNumber);
    sample->setGlobalStreamId(entry->globalStreamId);

    // Timing information
    sample->setGenerationTime(entry->generationTime);
    sample->setWriterArrivalTime(entry->writerArrivalTime);
    sample->setWriterFirstInjectionTime(entry->writerFirstInjectionTime);
    sample->setSubscriberReceiveTime(simTime());

    // QoS information
    sample->setDeadlineViolation(entry->deadlineViolation);
    sample->setLifespanExpired(entry->lifespanExpired);
    sample->setHistorySizeExeeded(entry->historySizeExeeded);

    // Enity information
    sample->setReaderParticipantEntityID(getParentModule()->par("participantId"));
    sample->setReaderEntityId(this->entityId);
    sample->setWriterParticipantEntityID(entry->writerParticipantEntityId);
    sample->setWriterEntityId(entry->writerEntityId);

    /* Send the message to the join_readers component */
    send(sample,"sinkOut");

}











