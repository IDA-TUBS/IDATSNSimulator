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

package idaTSNSimulator.endnode.middleware.rtps;

import idaTSNSimulator.endnode.contract.IMiddleware;
import idaTSNSimulator.endnode.middleware.rtps.contract.IAdapter;
import idaTSNSimulator.endnode.middleware.rtps.contract.IShaper;

import idaTSNSimulator.endnode.middleware.rtps.delayunit.DelayUnit;
import idaTSNSimulator.endnode.middleware.rtps.entities.Writer;
import idaTSNSimulator.endnode.middleware.rtps.entities.Reader;
import idaTSNSimulator.endnode.middleware.rtps.adapters.JoinReaders;

import idaTSNSimulator.endnode.middleware.rtps.shaper.Shaper;
import idaTSNSimulator.endnode.middleware.rtps.participant.Participant;

import idaTSNSimulator.endnode.middleware.rtps.adapters.RtpsToEthernetAdapter;

//
// The module containing the RTPS entities
//
module Rtps like IMiddleware
{
    parameters:

        /// Participant recognition
        int participantId = default(-1);

        /// Specify the number of RTPS reader and writer
        int numWriter = default(0);
        int numReader = default(0);


        // Port variables
//        int PB = default(7400);
//        int DG = default(250);
//        int PG = default(2);
//        int d0 = default(0);
//        int d1 = default(10);
//        int d2 = default(1);
//        int d3 = default(11);
//        int domainId = default(0);

        @class(Rtps);
        @display("i=block/layer;bgb=450,421");

    gates:

        /// Generator inputs connected to the writers
        input generatorIn[];
        /// Output to the sample sink
        output sinkOut;

        /// Connection to and from the UDP/IP stack
        input udpIpIn;
        output udpIpOut;


    submodules:

        /// The participant which serves as a message dispatcher
        participant: Participant {
            participantId = participantId;
            @display("p=251,221");
        }

        /// The RTPS writer instances publishing samples
        writer[numWriter]: Writer {
            @display("p=128,129,row,150");
        }

        /// The delay unit adds artificial delay before delivering the sample to the corresponding wirter
        delayUnit[numWriter]: DelayUnit {
            @display("p=128,47");

        }

        /// The RTPS reader instances that receive (have subscribed to) samples
        reader[numReader]: Reader {
            //  @display("p=366,129, row, 100");
            @display("p=379,129,row,150");
        }

        /// Join the readers to deliver to the shared sample source
        joinReaders: JoinReaders {
            @display("p=379,47");
        }

        /// The shaper shapes messages of all readers and writers before delivering them to the UDP/IP Stack
        shaper: <default("Shaper")> like IShaper {
            @display("p=116,277");
        }

        /// The transport adapter adds the relevant content to the RTPSEthernetFrame in order to be compatible with the underlying Ethernet infrastructure
        transportAdapter: <default("RtpsToEthernetAdapter")> like IAdapter {
            @display("p=251,345");
        }

    connections:

        /// Connect the stream chains (Sample generator, delay unit, writer)
        for i=0..numWriter-1 {
            /// Connect the writer with the dispatcher
            participant.writerIn++ <-- writer[i].dispatcherOut;
            participant.writerOut++ --> writer[i].dispatcherIn;

            /// Connect the generator with the delay unit
            generatorIn++ --> delayUnit[i].in;

            /// Connect the delay unit with the writer            
            delayUnit[i].out --> writer[i].appIn;

        }

        /// Connect the readers 
        for i=0..numReader-1 {
            /// Connect the readers with the dispatcher
            participant.readerIn++ <-- reader[i].dispatcherOut;
            participant.readerOut++ --> reader[i].dispatcherIn;

            /// Join the readers output
            reader[i].sinkOut --> joinReaders.readersIn++;

        }
        /// Connection the joined readers with the sample sink
        joinReaders.sinkOut --> sinkOut;

        /// Connect the send path
        participant.shaperOut --> shaper.dispatcherIn;
        shaper.adapterOut --> transportAdapter.shaperIn;
        transportAdapter.udpSocketOut --> udpIpOut;

        /// Connect the receive path
        udpIpIn --> transportAdapter.udpSocketIn;
        transportAdapter.dispatcherOut --> participant.adapterIn;



}
