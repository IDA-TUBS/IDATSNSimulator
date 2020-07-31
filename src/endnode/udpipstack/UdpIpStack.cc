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

#include "../../endnode/udpipstack/UdpIpStack.h"

#include "../../common/ValidationHandler.h"
#include "../../endnode/middleware/rtps/messages/RTPSEthMessage_m.h"
#include "../../messagetype/EthernetFrame_m.h"
#include <string.h>
#include <sstream>

UdpIpStack::UdpIpStack()
{
}

UdpIpStack::~UdpIpStack()
{
}

void UdpIpStack::initialize()
{
}


void UdpIpStack::handleMessage(cMessage* msg)
{
    // Check for RTPS message
    if (dynamic_cast<RTPSEthMessage*>(msg)!=NULL)
    {

        // Forwarding to the middleware module when received from an Ethernet port
        if(msg->arrivedOn("ingressPortIn"))
        {
            send(msg, "mwOut");
            return;
        // Forwarding to the Ethernet network port when received from the middleware module
        } else if(msg->arrivedOn("middlewareIn"))
        {
            RTPSEthMessage* rtpsEthMessage = dynamic_cast<RTPSEthMessage*>(msg);

            // First get the parent Module
            cModule* parent = this->getParentModule();

            // Iterate through the modules ...
            for (cModule::SubmoduleIterator it(parent); !it.end(); ++it) {
                cModule *submodule = *it;

                // ... and filter for egress ports
                std::string str = submodule->getFullName();
                if (str.find("egressPort") == std::string::npos) {
                    continue;
                }

                // Then get the mac value of the egress port
                int mac = submodule->par("mac");

                // Check if it is the correct egress port
                if(mac!=rtpsEthMessage->getSourceMac()){
                    continue;
                }

                // Now send the message to the correct port
                send(msg, "egressPortOut",submodule->getIndex());

                return;
            }

            std::stringstream ss;
            ss << "No matching EgressPort found for the given MAC-address " << rtpsEthMessage->getSourceMac() << ". Check your Source-MAC configuration.";
            ValidationHandler::ThrowError(ss.str().data());

        } else
        {
           // There are no other options, throw error
           ValidationHandler::ThrowError("Where did the RTPSEthMessage come from?");
        }

    // If pure Ethernet message
    }  else if(dynamic_cast<EthernetFrame*>(msg)!=NULL)
    {

        // Pass arrived frames to the pure Ethernet sink
        if (msg->arrivedOn("ingressPortIn"))
        {
            send(msg, "sinkOut");
        // Pass generated frames to the Ethernet network ports
        } else if (msg->arrivedOn("generatorIn"))
        {
           // Arrived from the generator
           send(msg, "egressPortOut", 0);
        } else if (msg->arrivedOn("middlewareIn"))
        {
            ValidationHandler::ThrowError("Where did it come from?");
        }
        else
        {
           //there are no other options, throw error
           ValidationHandler::ThrowError("Where did the EthernetFrame come from?");
        }


    } else
    {
        ValidationHandler::ThrowError("ERROR: Unknown message type in Ethernet UdpIpStack");
    }

}
