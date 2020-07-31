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

#include "SampleSink.h"

Define_Module(SampleSink);

void SampleSink::initialize()
{
    myMac = par("mac");
}


void SampleSink::handleMessage(cMessage *msg)
{

    if (dynamic_cast<Sample*>(msg)==NULL){
        delete msg;
        return;
    }

    //cast to ETH message to access fields
    Sample *sample = check_and_cast<Sample *>(msg);

    StatisticManager::getInstance().getSampleLatencyStatistics()->addMessage(
            sample->getReaderParticipantEntityID(), sample->getReaderEntityId(), sample->getGlobalStreamId(),
            sample->getSubscriberReceiveTime() - sample->getWriterFirstInjectionTime());

    delete msg;
}
