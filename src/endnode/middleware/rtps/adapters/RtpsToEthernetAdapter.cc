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

#include "../../../../endnode/middleware/rtps/adapters/RtpsToEthernetAdapter.h"

#include "./../messages/RTPSEthMessage_m.h"
#include "./../../../../common/EthernetMacros.h"
#include "./../../../../common/ValidationHandler.h"
#include "../../../../common/RtpsMacros.h"

Define_Module(RtpsToEthernetAdapter);

RtpsToEthernetAdapter::RtpsToEthernetAdapter(){
    frameSequenceId = 0;
}

RtpsToEthernetAdapter::~RtpsToEthernetAdapter()
{
}

void RtpsToEthernetAdapter::initialize()
{
}
unsigned long long RtpsToEthernetAdapter::getNextFrameSequenceId()
{
    // Return and increase the frame count
    return frameSequenceId++;
}

void RtpsToEthernetAdapter::finish()
{
    // Print information about the total number of send frames via this adapter
    EV << "RTPS to Ethernet Adapter has send " << getNextFrameSequenceId() << " frames!\n";
}

void RtpsToEthernetAdapter::handleMessage(cMessage *msg)
{
    // The adapter can only handle messages from type RTPSEthMessage
    if(dynamic_cast<RTPSEthMessage*>(msg)==NULL){
        ValidationHandler::ThrowError("Error: Invalid message type received in RtpsToEthernetAdapter!");
    }

    // Check arrival gates
    if(msg->arrivedOn("udpSocketIn"))
    {
        // Always pass the message from the socket to the dispatcher.
        send(msg, "dispatcherOut");
        return;

    } else if(msg->arrivedOn("shaperIn"))
    {
        // Cast and adapt the message via the sendRtpsPacket message
        RTPSEthMessage *packet = check_and_cast<RTPSEthMessage*>(msg);
        sendRtpsPacket(packet);
        return;
    }

    ValidationHandler::ThrowError("Error: Unreachable region!");

}

void RtpsToEthernetAdapter::sendRtpsPacket(RTPSEthMessage *packet)
{

    // Naming and addressing
    packet->setName(this->getParentModule()->getParentModule()->getName());
    packet->setMACsource(packet->getSourceMac());
    packet->setMACdest(packet->getDestinationMac());

    // Simulation context
    packet->setInjectionTime(simTime());
    int myKind = _std_eth_colour_;
    packet->setKind(myKind);


    // Broadcast configuration
    // XXX False per default right now
    packet->setIsBroadcast(false);

    // Multicast
    // XXX Always true by now since the routing is defined explicitly on switch level
    packet->setIsMulticast(true);

    // Size configuration
    int rtpsMessageSize = packet->getRtpsMessageSize();
    int rtpsUdpIpMessageSize = _rtps_add_udp_ip_overhead_(rtpsMessageSize);
    // Add the UDP/IP Overhead
    int ethernetPayloadLength = _eth_check_for_max_payload_(_eth_check_for_padding_(rtpsUdpIpMessageSize));
    // Check for size validity
    if(ethernetPayloadLength < rtpsUdpIpMessageSize){
        ValidationHandler::ThrowError("ERROR: RTPS message size exeeds the capabilities of a single Ethernet Frame!");
    }
    packet->setPayloadSize(rtpsUdpIpMessageSize);
    packet->setFrameSize(_eth_framesize_from_payload_(rtpsUdpIpMessageSize));
    packet->setPacketSize(_eth_packetsize_from_payload_(rtpsUdpIpMessageSize));

    // Assign the global unique Stream Id
    packet->setStreamID(packet->getGlobalStreamId());

    // Calculate the unique frame ID
    packet->setFrameSequenceID(getNextFrameSequenceId());

    _adapter_src_verbose_
    {
        EV << "Ethernet message context" << "\n";
        EV << "frameSize         : " << packet->getFrameSize()         << " -- \n";
        EV << "payloadSize       : " << packet->getPayloadSize()       << " -- \n";
        EV << "packetSize        : " << packet->getPacketSize()        << " -- \n";
        EV << "MACsource         : " << packet->getMACsource()         << " -- \n";
        EV << "MACdest           : " << packet->getMACdest()           << " -- \n";
        EV << "priority          : " << packet->getPriority()          << " -- \n";
        EV << "streamID          : " << packet->getStreamID()          << " -- \n";
        EV << "isMulticast       : " << packet->getIsMulticast()       << " -- \n";
        EV << "isBroadcast       : " << packet->getIsBroadcast()       << " -- \n";
        EV << "frameSequenceId   : " << packet->getFrameSequenceID()   << " -- \n";
    }

    // Send out the message to the socket
    send(packet,"udpSocketOut");

}

























