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

#include "../msgcreators/SampleMsgCreator.h"

#include "../../../common/GlobalMacros.h"
#include "../../../common/RtpsMacros.h"
#include "./../../../common/ValidationHandler.h"

#include "../../../endnode/middleware/rtps/messages/Sample_m.h"


SampleMsgCreator::SampleMsgCreator()
{
}

SampleMsgCreator::~SampleMsgCreator()
{
}


void SampleMsgCreator::initialize()
{
    // Start counting Sample IDs (Sequence Numbers) at 0
    sampleSequenceNumberId = 0;
}

inline int64_t SampleMsgCreator::getNextSampleSequenceNumberId()
{
    // Return the current value and Increase the variable thereafter
    return sampleSequenceNumberId++;
}



void SampleMsgCreator::handleMessage(cMessage *msg)
{

    // Create new sample
    Sample* topicSample = new Sample();

    // Calculate the sample message payload size
    int sampleSize = int(getParentModule()->par("sampleSize").doubleValue());
    sampleSize = truncate_min_max<int>(sampleSize, getParentModule()->par("sampleSizeMin").intValue(),
            getParentModule()->par("sampleSizeMax").intValue());

    // Makes sure the Sample size is neither too large
    if(sampleSize > _MAX_RTPS_PAYLOAD_){
        ValidationHandler::ThrowError("Sample size exceeds the maximum allowed value!");
    }

    // Pass the values to the topic sample
    topicSample->setName(this->getParentModule()->getParentModule()->getName());
    topicSample->setSize(sampleSize);
    topicSample->setSequenceNumber(getNextSampleSequenceNumberId());
    topicSample->setGlobalStreamId(getParentModule()->par("globalStreamId"));
    topicSample->setGenerationTime(simTime());

    // Last send the sample
    send(topicSample, "toStack");
    delete msg;
}

void SampleMsgCreator::finish()
{
    int sID = this->getParentModule()->par("globalStreamId");
    int number = getNextSampleSequenceNumberId();
    EV << "Generator wit stream ID " << sID << " has created " << number << " samples!\n";
}
