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

package idaTSNSimulator.simulations2020.simpletest;

import idaTSNSimulator.endnode.EndNode;
import idaTSNSimulator.links.ethernetChannel1Gbps;
import idaTSNSimulator.switch.Switch;


network simpleTest
{
    submodules:
        switch: Switch 
        {
            @display("p=158,103");
        }
        endNode: EndNode 
        {
            @display("p=42,103");
        }
        endNode1: EndNode 
        {
            @display("p=275,103");
        }
    connections:
        endNode.ethernetPorts++ <--> ethernetChannel1Gbps <--> switch.ethernetPorts++;
        switch.ethernetPorts++ <--> ethernetChannel1Gbps <--> endNode1.ethernetPorts++;
}

