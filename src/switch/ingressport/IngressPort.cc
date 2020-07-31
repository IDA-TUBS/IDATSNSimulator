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

#include "IngressPort.h"

/**
 * Constructor
 */
IngressPort::IngressPort()
{

}

/**
 * Destructor
 */
IngressPort::~IngressPort()
{
    cancelAndDelete(this->triggerSelf);
}

/**
 *
 */
void IngressPort::initialize()
{
    this->macAddress = par("mac");
    this->ingressPortID = getIndex();
    this->sampleTimePeriod = par("sampleTimePeriod");
    this->numBytes = 0;

    //statistics vector for collecting the link utilization for a resolution given by the sampleTimePeriod variable
    char name[32];
    sprintf(name, "link-utilization-bytes");
    this->linkUtilizationStatistics.setName(name);

    //mesagge triggering statistics collection
    this->triggerSelf->setName("triggerUtilizationStats");
    scheduleAt(simTime() + this->sampleTimePeriod, this->triggerSelf);
}

/**
 *
 */
void IngressPort::handleMessage(cMessage* message)
{
    if(message->isSelfMessage())
    {
        dealWithInternalMessage(message);
    }
    else
    {
        dealWithExternalMessage(message);
    }
}

/**
 *
 */
void IngressPort::dealWithInternalMessage(cMessage* message)
{
    if (message == this->triggerSelf)
    {
        linkUtilizationStatistics.record(this->numBytes);
        scheduleAt(simTime() + this->sampleTimePeriod, this->triggerSelf);
        this->numBytes = 0;
    }
}

/**
 *
 */
void IngressPort::dealWithExternalMessage(cMessage* message)
{
    EthernetFrame* ethernetFrame = checkAndCastEthernetFrame(message);
    dealWithEthernetFrame(ethernetFrame);
}

/**
 *
 */
EthernetFrame* IngressPort::checkAndCastEthernetFrame(cMessage* message)
{
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*>(message);
    if (ethernetFrame == nullptr)
    {
        const char* moduleName = getFullName();
        EV <<"[" << moduleName << "] Could not cast to EthernetFrame* \n";
        throw cRuntimeError("[IngressPort] Could not cast to EthernetFrame* \n");
    }
    return ethernetFrame;
}

/**
 *
 */
void IngressPort::dealWithEthernetFrame(EthernetFrame* ethernetFrame)
{
    //gather information for utilization statistics
    this->numBytes = this->numBytes + ethernetFrame->getPacketSize();
    //Forward to the next module within the switch
    send(ethernetFrame, "out");
}

