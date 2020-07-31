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

package idaTSNSimulator.endnode;

import idaTSNSimulator.endnode.generator.*;
import idaTSNSimulator.endnode.sink.*;
import idaTSNSimulator.endnode.contract.IMiddleware;
import idaTSNSimulator.endnode.udpipstack.UdpIpStack;
import idaTSNSimulator.endnode.middleware.rtps.Rtps;
import idaTSNSimulator.switch.egressport.EgressPort;
import idaTSNSimulator.switch.ingressport.IngressPort;
//
// The endnode is the high-level module for conncetion to the network.
// Therefore it enables to send or receive Ethernet or RTPS message streams via dedicated Ethernet ports.
//
module EndNode
{
    parameters:
        
        /// The number of Middleware Writers starting with one Sample Generator each.
        int numMwStreams = default(0);
        
        /// The number of Middleware Readers represented by one Reader each.
        int numMwSinks = default(0);

        /// The number of pure Ethernet packet streams starting with one Ethernet Generator each.
        int numStreams = default(0);

        /// The default MAC Address of the endnode
        int mac;
        
        /// Ethernet Port configurations
        /// Standard Ethernet supports up to 8 priorities
        int supportedPriorities = default(8);
        int maxMsgPerQueue = default(-1); // -1 = ignore this restriction
        int maxBytesPerQueue = default(4096); // 4kbyte of buffer per port, -1 = ignore this restriction

        @display("bgb=680,419;i=block/rxtx");

    gates:
        
        // In- Out- connections to the Ethernet Network
        inout ethernetPorts[];

    submodules:

        /// Ethernet Ports
        egressPort[sizeof(ethernetPorts)]: EgressPort 
        {
            @display("p=477,319,row,100");
        }
        ingressPort[sizeof(ethernetPorts)]: IngressPort 
        {
            @display("p=235,319,row,100");
        }

        /// The UDP-IP Stack to forward messages between the middleware, Ethernet generators and sinks and the Ethernet ports
        udpIpStack: UdpIpStack 
        {
            @display("p=445,211");
        }

        /// The middleware
        middleware: <default("Rtps")> like IMiddleware 
        {
            numWriter = numMwStreams;
            numReader = numMwSinks;
            @display("p=294,146");
        }

        /// Generators
        // Pure ethernet generators
        generator[numStreams]: EthGenerator 
        {
            @display("p=438,58,r,50");
        }

        // Middleware sample generators
        sampleGenerator[numMwStreams]: sampleGenerator 
        {

            @display("p=124,58");
        }     
        
        /// Sinks
        /// Middleware sample sink
        sampleSink: SampleSink 
        {
            mac = mac;
            @display("p=285,58");
        }

        /// Pure Ethernet sink
        sinkPureEthernet: Sink 
        {
            mac = mac;
            @display("p=589,58");
        }

    connections:

        /// Connect the generators
        for i=0..numMwStreams-1 
        {
            sampleGenerator[i].out --> middleware.generatorIn++;
        }
        for i=0..numStreams-1
        {
            generator[i].out --> udpIpStack.generatorIn++;
        }

        /// Connect the Ports to the network and the UDP/IP stack
        for i=0..sizeof(ethernetPorts)-1 
        {
            ethernetPorts$i[i] --> ingressPort[i].in;
            egressPort[i].out --> ethernetPorts$o[i];

            udpIpStack.egressPortOut++ --> egressPort[i].in++;
            ingressPort[i].out --> udpIpStack.ingressPortIn++;
        }

        /// Connect the UDP/IP Stack
        /// Connect the UDP/IP Stack with the pure Ethernet sink
        udpIpStack.sinkOut --> sinkPureEthernet.in;
        // Connect the UDP/IP Stack with the middleware
        udpIpStack.mwOut --> middleware.udpIpIn;
        middleware.udpIpOut --> udpIpStack.middlewareIn;


        /// Connect the sample sink to the middleware
        middleware.sinkOut --> sampleSink.in;


}
