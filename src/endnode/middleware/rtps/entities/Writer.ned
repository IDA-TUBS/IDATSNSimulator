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

//
// The RTPS Writer entity sends generated samples to its subscribed readers based on the streamID routing mechanism implemented in the network.
//
simple Writer
{
    parameters:
        
        // Stream configuration
        int sourceMac;
        int destinationMac;
        int entityId;
        int destinationEntityId;
        int streamEthernetPriority;
        int globalStreamId = default(0); // Should match to the corresponding globalStreamId of the writer
                
        // QoS parameter configuration
        int historySizeQoS = default(1); // Number of stored Samples on the publisher side        
        int transportPriority = default(7); 
        int ethernetPriority = default(7); 
        bool bestEffort = default(true);
        double lifespan @unit(s) = default(100ms); 

        // Fragmentation configuration
        int fragmentSize = default(1000);
        int maximumMtu = default(1504);

        // Shaping
        double shapingDuration @unit(s) = default(100us);

        // Display Parameters
        @display("i=block/tx");
        
    gates:
        input appIn;
        input dispatcherIn;
        output dispatcherOut;
        
        
}
