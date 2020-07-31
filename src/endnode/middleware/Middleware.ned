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

package idaTSNSimulator.endnode.middleware;

//
// The middleware interface specifying the placement in the endnode
//
moduleinterface Middleware
{    
    parameters:
        /// One MAC address of the module // XXX Future cleanup: Put this directly in the ports, messages and sinks
        int mac;
    gates:     
        /// Connection to the generators
        input  generatorIn[];
        /// Connection to the sink
        output sinkOut;
        
        /// Connection to the ingress ports
        input ingressPortIn[];
        /// Connection from the egress ports
        output egressPortOut[]; 
}
