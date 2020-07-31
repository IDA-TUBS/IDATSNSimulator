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
// The EgressPort is a module which include the classifier, buffersmoudle and the arbiter module to process and store forwarded frames
// based on the selected classifier and arbiter the behaviour can be influenced
//
module EgressPort
{
    parameters:
        /// The default MAC Address
        int mac 				= default(0);
        /// the default number of Priorities
        int supportedPriorities = default(8);
        // default arbiter 
        string arbiterType = default("ArbiterIEEE802_1Q");
        @display("i=block/queue;bgb=500,250");
        
    gates:
        input in[];
        output out;

    submodules:
        classifier: <default("ClassifierPriority")> like Classifier 
        {
            supportedPriorities = supportedPriorities;
            @display("p=70,100");
        }
        
        buffer: BuffersModule
        {
            supportedPriorities = supportedPriorities;
            @display("p=250,100");
        }
        
        arbiter: <arbiterType> like Arbiter 
        { 
            supportedPriorities = supportedPriorities;
            @display("p=430,100");
        }
        
    connections:
        for i=0..sizeof(in)-1 
        {
            in[i] --> classifier.in++;
        }
        for j=0..supportedPriorities-1 
        {
            classifier.out[j] --> buffer.in[j];            
        }
        buffer.out --> arbiter.in;
        buffer.control <--> arbiter.control;
        arbiter.out --> out;
}


