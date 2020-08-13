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

package idaTSNSimulator.endnode.generator;

import idaTSNSimulator.endnode.generator.msgcreators.*;
import idaTSNSimulator.endnode.generator.trafficsrcs.*;

//
// This modules combines the Traffic source (stimulus) and the raw Ethernet message creator to implement and configure an Ethernet stream generator
//
module TraceGenerator
{
    parameters:
        
        		
		//The maximum size of the trace, if zou would like to change this value be sure that you checked the impact on the performance.
		int maxTraceLength = default(50000);
        
        //This variable decides if the payload values are taken from trace of statically configured
        bool payloadTrace = default(false);
        
        double scaleFactor = default(1);

        /// Parameters for the generator
        bool isEnabled = default(true);
        int globalStreamId = default(-1);
        int destinationMac = default(-1); // The MAC address to send the message to.
        int ethernetPriority = default(-1);
        bool isMulticast = default(false);
        string 	fileName;					 // trace filename
        
        
        /// Offset (if enabled)
        bool enableOffset = default(false); // Delays the first burst if true
        double offsetDuration @unit(s) = default(0s); // The duration of the offset
        
        /// Parameters for the srcTopicSampleHelper
        /// The size of a packet         
        volatile double payloadSizeBase @unit(B) = default(0B); // The amount of data (payload) to be sent in each generated packet, in Bytes!
        int payloadSizeMin @unit(B) = default(0B); // Minimum payload (Bytes) If a probabilistic distribution is used.
        int payloadSizeMax @unit(B) = default(0B); // Maximum payload (Bytes) If a probabilistic distribution is used.
        
		
		/// Statistics
        bool enableFineGrainedLatencyStats = default(false);
		
        @display("i=block/departure");

    gates:
        
        /// The output for the generated messages
        output out;

    submodules:
        src: TraceSource
        {
            @display("p=97,48");
        }

        messageCreator: TraceEthMsgCreator 
        {
            enableFineGrainedLatencyStats = enableFineGrainedLatencyStats;
            @display("p=97,119");
        }

    connections:

        src.messageGenerationOut --> messageCreator.fromSrc;
        messageCreator.toStack --> out;

}
