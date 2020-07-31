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

package idaTSNSimulator.switch.egressport;

//
// BuffersModule manages the buffers per port and includes an array of FrameBuffers (one Frame Buffer per individual traffic class / priority)
//
simple BuffersModule
{
    parameters:
        /// the default number of Priorities
        int supportedPriorities;
        /// the default size of each buffer per Priority
        int maxBytesPerQueue = default(4096);
        @display("i=block/queue;bgb=513,275");
        
    gates:
        input in[supportedPriorities];
        output out;    
        inout control;
}
