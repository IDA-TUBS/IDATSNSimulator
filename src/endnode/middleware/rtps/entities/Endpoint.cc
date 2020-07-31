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

#include "../../../../endnode/middleware/rtps/entities/Endpoint.h"

#include "./../../../../common/ValidationHandler.h"
#include "../../../../common/RtpsMacros.h"



unsigned long long Endpoint::getNextUniqueRtpsMsgId()
{
    // Return and increase the RTPS message count of the entity
    return rtpsMsgSequenceId++;
}

void Endpoint::calculateRtpsMsgSize(RTPSEthMessage* rtpsMsg)
{
    // Initialize the counting parameter
    int cummulatedSize = 0;

    // Add the RTPS header size
    cummulatedSize += _RTPS_HEADER_SIZE_;

    // DataFrag
    if(rtpsMsg->getDataFragIsSet())
    {
        cummulatedSize += _RTPS_DATAFRAG_STATIC_SIZE_;
        if(rtpsMsg->getFragmentSize() > _RTPS_MAX_FRAG_SIZE_)
        {
            ValidationHandler::ThrowError("Maximum RTPS fragment size exeeded!");
        }
        cummulatedSize += rtpsMsg->getFragmentSize();
    }

    // HeartBeatFrag
    if(rtpsMsg->getHeartBeatFragIsSet())
    {
        cummulatedSize += 28;
    }

    // InfoDestination
    if(rtpsMsg->getInfoDestinationIsSet())
    {
        cummulatedSize += _RTPS_INFO_DST_SIZE_;
    }

    // InfoTimestamp
    if(rtpsMsg->getInfoTimestampIsSet())
    {
        cummulatedSize += _RTPS_INFO_TS_SIZE_;
    }

    // NackFrag
    if(rtpsMsg->getNackFragIsSet())
    {
        int bitmapSizeBytes = (rtpsMsg->getFragmentNumberStateNbrBits()+7)/8;
        bitmapSizeBytes = ((bitmapSizeBytes + 3)/4)*4;
        cummulatedSize += _RTPS_NACKFRAG_STATIC_SIZE_ + bitmapSizeBytes;
    }

    if(rtpsMsg->getDataIsSet() || rtpsMsg->getAckNackIsSet() || rtpsMsg->getHeartBeatIsSet()){
        ValidationHandler::ThrowError("Unknown message for RTPS message size calculation!");
    }

    // Finally set the cummulated RTPS message size
    rtpsMsg->setRtpsMessageSize(cummulatedSize);
}
