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

#include "LinkTransmissionStatistics.h"


LinkTransmissionStatistics::LinkTransmissionStatistics()
{
}

LinkTransmissionStatistics::~LinkTransmissionStatistics()
{
}

void LinkTransmissionStatistics::finish()
{
    if (this->activationPeriod)
    {
        this->activationPeriod->finish();
        delete this->activationPeriod;
        this->activationPeriod = nullptr;
    }
}


void LinkTransmissionStatistics::ethernetFrameStartedSending(int switchMAC, int portIndex, int streamID)
{
    if (this->activationPeriod == 0)
    {
        this->activationPeriod = new VectorStatistics3DMap("Transmission", "SW", "port", "SID");
    }
    this->activationPeriod->addInteger(switchMAC, portIndex, streamID, 0);
    this->activationPeriod->addInteger(switchMAC, portIndex, streamID, 1);

}


void LinkTransmissionStatistics::ethernetFrameFinishedSending(int switchMAC, int portIndex, int streamID)
{
    if (this->activationPeriod == 0)
    {
        this->activationPeriod = new VectorStatistics3DMap("Transmission", "SW", "port", "SID");
    }
    this->activationPeriod->addInteger(switchMAC, portIndex, streamID, 1);
    this->activationPeriod->addInteger(switchMAC, portIndex, streamID, 0);
}


