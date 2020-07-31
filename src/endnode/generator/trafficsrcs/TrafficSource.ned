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

package idaTSNSimulator.endnode.generator.trafficsrcs;

//
// Implementation of the stimuli-generating component based on a periodic burst model
//
simple TrafficSource
{
    parameters:
        @display("i=block/departure");
    gates:
        /// The port to the message creators (Samples or pure Ethernet)
        output messageGenerationOut;
}
