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

#include "../msgcreators/TraceEthMsgCreator.h"

#include "../../../common/GlobalMacros.h"
#include "../../../common/EthernetMacros.h"
#include "../../../messagetype/EthernetFrame_m.h"
#include "../../../messagetype/TraceGenerateMessage_m.h"


TraceEthMsgCreator::TraceEthMsgCreator()
{
}


TraceEthMsgCreator::~TraceEthMsgCreator()
{
    // Nothing to clean up.
}


void TraceEthMsgCreator::initialize()
{
    // Load input parameters
    enableFineGrainedLatencyStats = par("enableFineGrainedLatencyStats").boolValue();

    // Start counting IDs at 0
    uniqueFrameId = 0;

}


inline int64_t TraceEthMsgCreator::getNextUniqueFrameId()
{
    // Return and increase the Id
    return uniqueFrameId++;
}


void TraceEthMsgCreator::handleMessage(cMessage *msg)
{

    // Create an Ethernet message
    EthernetFrame* ethMsg = new EthernetFrame();

    // Configure the created Ethernet message
    // The color of the Ethernet frame
    int kind = _std_eth_colour_;
    ethMsg->setInjectionTime(simTime());
    ethMsg->setStreamID(this->getParentModule()->par("globalStreamId"));
    ethMsg->setKind(kind);
    ethMsg->setMACsource(getParentModule()->getParentModule()->par("mac"));
    ethMsg->setMACdest(getParentModule()->par("destinationMac"));
    ethMsg->setName(this->getParentModule()->getParentModule()->getName());

    // Load the priority from the TraceEthMsgCreator parent module
    ethMsg->setPriority(getParentModule()->par("ethernetPriority"));

    // No broadcast by default right now
    ethMsg->setIsBroadcast(false);

    // Multicast is on by default
    ethMsg->setIsMulticast(getParentModule()->par("isMulticast"));

    // Set the Ethernet message payload size
    bool payloadTrace = this->getParentModule()->par("payloadTrace").boolValue();
//    EV << " Payload from trace "<< payloadTrace << "\n";
    if(payloadTrace != true)
    {
        int payloadLength = int(getParentModule()->par("payloadSizeBase").doubleValue());
        payloadLength = truncate_min_max<int>(payloadLength, getParentModule()->par("payloadSizeMin").intValue(),
        getParentModule()->par("payloadSizeMax").intValue());

        // Makes sure the Ethernet frame is neither too large nor too small
        payloadLength = _eth_check_for_max_payload_(_eth_check_for_padding_(payloadLength));
        ethMsg->setPayloadSize(payloadLength);
        ethMsg->setFrameSize(_eth_framesize_from_payload_(payloadLength));
        ethMsg->setPacketSize(_eth_packetsize_from_payload_(payloadLength));
    }
    else{
        int payloadLength = 0;
        ethMsg->setPayloadSize(payloadLength);
        ethMsg->setFrameSize(_eth_framesize_from_payload_(payloadLength));
        ethMsg->setPacketSize(check_and_cast<TraceGenerateMessage *>(msg)->getPacketSize());
    }
    // Set the uniqueFrameId and increment it
    ethMsg->setFrameSequenceID(getNextUniqueFrameId());

    // Send out the Ethernet message over the Stack towards the switched network
    send(ethMsg, "toStack");

    // Last delete the stimulation message, since it is not further needed
    delete msg;
}

void TraceEthMsgCreator::finish()
{
    // Print out the number of generated frames of the generated stream
    int sID = this->getParentModule()->par("globalStreamId");
    int64_t number = getNextUniqueFrameId();
    EV << "Generator for stream " << sID << " has created " << number << " frames!\n";
}
