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

package idaTSNSimulator.switch;

import idaTSNSimulator.switch.ingressport.IngressPort;
import idaTSNSimulator.switch.forwardinglogic.ForwardingLogic;
import idaTSNSimulator.switch.egressport.EgressPort;

//
// The Switch is the central Module which includes a Ingress Port, Forwarding Logic and 
// Egress Port to process, store and forward Frames
//
module Switch
{
    parameters:
        /// The default MAC Address
        int mac 				= default(0);
        /// the default number of Priorities
        int supportedPriorities = default(8);
        /// default path to MAC Table
        string macTable 		= default("");
        @display("bgb=380,380;i=block/switch");
        
    gates:
        inout ethernetPorts[];
        
    submodules:
        ingressPort[sizeof(ethernetPorts)]: IngressPort 
        {
            mac = mac;
            @display("p=50,70,c,50");
        }  
        
        egressPort[sizeof(ethernetPorts)]: EgressPort 
        {
            mac = mac;
            supportedPriorities = supportedPriorities;
            @display("p=313.75,70,c,50");
        }
        
        forwardingLogic: ForwardingLogic 
        {
            @display("p=180,70");
        }
        
    connections:
        for i=0..sizeof(ethernetPorts)-1 
        {
            //in -> ports
            ethernetPorts$i[i] --> ingressPort[i].in;
			//port -> frer_elim
            ingressPort[i].out --> forwardingLogic.in++;

            //forwarding -> frer_repl
            forwardingLogic.out++ --> egressPort[i].in++;
			//ports -> out
            egressPort[i].out --> ethernetPorts$o[i];
        }
}

