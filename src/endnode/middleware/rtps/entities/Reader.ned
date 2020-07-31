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

package idaTSNSimulator.endnode.middleware.rtps.entities;

///The RTPS Readers entity resceives generated samples from RTPS writers.
simple Reader
{
    parameters:

        /// The MAC address of the egress port to use
        int sourceMac;
        /// The own entity ID
        int entityId;
        /// The used Ethernet priority for messages send by the reader
        int streamEthernetPriority = default(7);

        /// QoS parameters
        int historySizeQoS = default(10);
        bool bestEffort = default(true);
        double lifespan @unit(s) = default (100ms);
        double deadline @unit(s) = default(1000us);
        int transportPriority = default(0);
        
        @display("i=block/rx");
    gates:
        
        /// Gate to the sink via the reader join
        output sinkOut;
        
        /// Connection with the participant
        input dispatcherIn;
        output dispatcherOut;

}
