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

#include "Sink.h"

#include <sstream>


void Sink::initialize()
{
    this->macAddress = par("mac");
}

inline bool Sink::checkDestinationsMAC(EthernetFrame* ethMsg)
{
    return ethMsg->getMACdest() == this->macAddress;
}

void Sink::checkForMissingFrame(EthernetFrame* ethMsg)
{


    int streamID = ethMsg->getStreamID();
    int sequenceNo = ethMsg->getFrameSequenceID();

    EV << "Running checkForMissingFrame! StreamID: " << streamID << "SequenceNo: "<< sequenceNo << "\n" ;
    if (this->lostFrameCounters.count(streamID) == 0)
    {
        //first frame arrived of this stream
        this->lostFrameCounters[streamID] = 0;
        this->trackMissingDataMap[streamID] = -1;
    }

    if (sequenceNo == this->trackMissingDataMap[streamID]  + 1)
    {
        //this is the expected frame
        this->trackMissingDataMap[streamID]++;
    }
    else if(sequenceNo > this->trackMissingDataMap[streamID] + 1)
    {
        //a frame in the future, store the ones missing in the middle
        int counter = 0;
        for (int i = this->trackMissingDataMap[streamID]; i < sequenceNo; i++)
        {
            //store this frame as missing
            this->lostFrameCounters[streamID]++;
            counter++;
        }

        this->trackMissingDataMap[streamID] = sequenceNo;

        StatisticManager::getInstance().getFramesLostStatistics()->addLostFrame(this->macAddress,streamID, this->lostFrameCounters[streamID]);
        StatisticManager::getInstance().getFramesLostStatistics()->addLostFrameTrace(this->macAddress,streamID, counter);

    }else{
        this->lostFrameCounters[streamID]--;
        StatisticManager::getInstance().getFramesLostStatistics()->addLostFrame(this->macAddress,streamID, this->lostFrameCounters[streamID]);
        StatisticManager::getInstance().getFramesLostStatistics()->addLostFrameTrace(this->macAddress,streamID, -1);


    }

}

void Sink::processEthernetFrameAtDestination(EthernetFrame* ethMsg)
{

    simtime_t e2ePathDelayForStream = ethMsg->getArrivalTime() - ethMsg->getInjectionTime();

    EV << "[Eth Sink] pure ETH frame,  delay was: " << e2ePathDelayForStream << "\n";

    // Process end-to-end delay
    if (e2ePathDelayForStream.dbl() > 0)
    {
        StatisticManager::getInstance().getDelayStatistics()->addE2ELatency(this->macAddress, ethMsg->getStreamID(), ethMsg->getPriority(), e2ePathDelayForStream);
    }
    else
    {
        EV << "NEGATIVE PATH DELAY!!!\n";
        EV << ">>" << e2ePathDelayForStream << " = " << ethMsg->getArrivalTime() << "-" << ethMsg->getInjectionTime() << "\n";
        ValidationHandler::ThrowError("[Eth Sink] negative path delay detected!");
    }

    checkForMissingFrame(ethMsg);

}

void Sink::handleMessage(cMessage *msg)
{

    //cast external Message to the Ethernet frame
    EthernetFrame *ethMsg = check_and_cast<EthernetFrame *>(msg);


    EV << "[Eth Sink] " << this->macAddress << "has received an ETH frame with SID" << ethMsg->getStreamID() << "at time" << simTime().dbl() << "\n";

    //check if the message has arrived at the right destination
    if (!checkDestinationsMAC(ethMsg))
    {
        // we have wrong destination
        EV_WARN << "[Eth Sink] message " << ethMsg->getFullName() << " has arrived at ";
        EV_WARN << "[Eth Sink] " << this->macAddress << " but was addressed for " << ethMsg->getMACdest() << "\n";
        EV_WARN << "[Eth Sink] message will be deleted, but statistics are still collected (as if arrived to the right address)\n";
    }

    processEthernetFrameAtDestination(ethMsg);


    delete msg;
}



void Sink::finish()
{
    StatisticManager::getInstance().finishAll();
}

