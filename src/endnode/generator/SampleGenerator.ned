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
// This modules combines the Traffic source (stimulus) and the RTPS Sample message creator to implement and configure an Ethernet stream generator
//
module sampleGenerator
{
    parameters:
        
        /// Parameters for the generator
        bool isEnabled = default(true);
        
        /// Legacy parameter. Fill here only for output evaluation of the generator
        int  globalStreamId = default(-1);
             
        /// The periodicity of a sample burst
        volatile double periodBase @unit(s) = default(-1s); // The distance between two sample bursts (period), in seconds.    
        double periodMin @unit(s) = default(0s); // Minimum period (seconds). If a probabilistic distribution is used.
        double periodMax @unit(s) = default(0s); // Maximum period (seconds). If a probabilistic distribution is used.
        
        /// Burst length in number of samples  
        volatile double burstSizeBase = default(1); // The burst length in number of samples.
        int burstSizeMin = default(1);  // Minimum burst length (nbr packets). If a probabilistic distribution is used.
        int burstSizeMax = default(0);  // Maximum burst length (nbr packets). If a probabilistic distribution is used.
        
        /// Intra burst period (distance between samples inside a burst)
        volatile double intraBurstDurationBase @unit(s) = default(0s); // The distance between packets inside a burst.
        double intraBurstDurationMax @unit(s) = default(0s); // Minimum distance (in seconds) between packets inside a burst. If a probabilistic distribution is used.
        double intraBurstDurationMin @unit(s) = default(0s); // Minimum distance (in seconds) between packets inside a burst. If a probabilistic distribution is used.

        /// Offset (if enabled)
        bool enableOffset = default(false); // Whether the burst starts at time 0s + offset (true) or at 0s + period (false)
        double offsetDuration @unit(s) = default(0s); // the value for offset, see ::useOffset!!!
           
           
        /// Parameters for the srcTopicSampleHelper
        /// The size of a sample
        volatile double sampleSize @unit(B) @display("i=block/departure") = default(0); // XXX Disable if 0!
        int sampleSizeMin @unit(B) = default(0B); // Minimum payload (Bytes) If a probabilistic distribution is used.
        int sampleSizeMax @unit(B) = default(0B); // Maximum payload (Bytes) If a probabilistic distribution is used.
        
        
        @display("i=block/departure");
    gates:
        
        /// The output for the generated messages
        output out;

    submodules:
        src: TrafficSource
        {
            @display("p=97.695,40.08");
        }

        msgCreator: SampleMsgCreator {
            @display("p=97.695,126.025");
        }

    connections:

        src.messageGenerationOut --> msgCreator.fromSrc;
        msgCreator.toStack --> out;

}
