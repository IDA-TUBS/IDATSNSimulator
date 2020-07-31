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

package idaTSNSimulator.endnode.sink;

//
//  Sink that gathers statistics for individula Ethernet fames and streams.
//
simple Sink
{
    parameters:
        /// One MAC Address of the endnode
        int mac;
        @display("i=block/sink");

    gates:
        /// The sinks input gate
        input in;
}
