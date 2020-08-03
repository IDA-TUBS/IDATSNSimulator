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

#ifndef SRC_ENDNODE_UDPIPSTACK_UDPIPSTACK_H_
#define SRC_ENDNODE_UDPIPSTACK_UDPIPSTACK_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * Passes the data from the middleware and pure Ethernet generators to the ethernet network ports.
 * Passes the data from the Ethernet network ports to either the middleware or the pure Ethernet sink.
 */
class UdpIpStack : public cSimpleModule
{
public:
    /**
     * Default constructor
     */
    UdpIpStack();

    /**
     * Default destructor
     */
    virtual ~UdpIpStack();
    /**
     * Empty initialization function
     */
    void initialize() override;
    /**
     * forwarding the messages between the Ethernet network ports and the middleware and pure Ethernet generators/sink
     *
     * @param msg The message to be forwarded
     */
    void handleMessage(cMessage* msg) override;

};

Define_Module(UdpIpStack);

#endif /* SRC_ENDNODE_UDPIPSTACK_UDPIPSTACK_H_ */
