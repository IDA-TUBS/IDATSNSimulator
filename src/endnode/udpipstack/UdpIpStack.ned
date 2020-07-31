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

package idaTSNSimulator.endnode.udpipstack;

//
// Passes the data from the middleware and pure Ethernet generators to the ethernet network ports.
// Passes the data from the Ethernet network ports to either the middleware or the pure Ethernet sink.
//
simple UdpIpStack
{
    parameters:
        @display("i=block/socket");

    gates:
        
        /// Connection to the middleware module
        input middlewareIn;
        /// Connection from the middleware module
        output mwOut;

        /// Connections from the pure Ethernet generators.
        input generatorIn[];
        /// Connection to the sink.
        output sinkOut;
        
        /// Input Connections from the Ethernet network
        input ingressPortIn[];
        
        /// Output Connections to the Ethernet network
        output egressPortOut[];
}
