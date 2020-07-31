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
// The ClassifierPriority is a Classifier based on priority filtering and is an interface for the Classifier Module.
//
simple ClassifierPriority like Classifier
{
    parameters:
        /// the default number of Priorities
        int supportedPriorities;
        @display("i=block/classifier2;bgb=500,250");
        
    gates:
        input in[];
        output out[supportedPriorities];
}
