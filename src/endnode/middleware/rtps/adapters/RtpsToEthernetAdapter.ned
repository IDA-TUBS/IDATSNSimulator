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


//
// The Adapter configures the Ethernet message related context to the rtps frame
//

package idaTSNSimulator.endnode.middleware.rtps.adapters;

import idaTSNSimulator.endnode.middleware.rtps.contract.IAdapter;

simple RtpsToEthernetAdapter like IAdapter
{
    @display("i=block/segm");
    gates:
        
        // Socket connection
        input udpSocketIn;
        output udpSocketOut;
        
        // Writer inputs
        input shaperIn;
        
        // Reader Outputs
        output dispatcherOut;
        
}
