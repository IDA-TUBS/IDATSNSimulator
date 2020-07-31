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

package idaTSNSimulator.links;

//
// Definitions of links used in the simulation e.g. latency, length etc.
//

channel ethernetLink extends ned.DatarateChannel
{
    datarate = default(100Mbps);
    double length @unit(m) = default(10m); //of copper wire
    double cInCopper @unit(m) = 200000000m;
    //50 ns are roughly equal to 10 meters of copper wire
    delay = this.length / this.cInCopper * 1s;
}

channel ethernetChannel100Mbps extends ethernetLink
{
    datarate = 100Mbps;
    @display("ls=,2");
}

channel ethernetChannel1Gbps extends ethernetLink
{
    datarate = 1Gbps;
    @display("ls=,4");
}

channel ethernetChannel500Mbps extends ethernetLink
{
    datarate = 500Mbps;
    @display("ls=,3");
}
