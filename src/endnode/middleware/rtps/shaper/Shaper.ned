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

package idaTSNSimulator.endnode.middleware.rtps.shaper;

import idaTSNSimulator.endnode.middleware.rtps.contract.IShaper;

//
// The shaper shapes outgoing Ethernet messages on middleware module granularity (for all readers and writers)
//
simple Shaper like IShaper
{
    parameters:

        /// Parameter to enable/disable the shaper
        bool shaperEnabled = default(false);
        
        /// The shaping duration
        double shapingDuration @unit(s) = default(10us); 

    @display("i=block/join");
    
    gates:
        
        /// Input gate for incoming messages
        input dispatcherIn;
        
        /// Pass all outgoing messages shaped (if enabled to the adapter)
        output adapterOut;
        



}
