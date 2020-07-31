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

package idaTSNSimulator.endnode.middleware.rtps.delayunit;

//
// Adds artificial delay to the delivery of the sample to the writer to model runtime overoverhead
//
simple DelayUnit
{
    parameters:
        
        /// Probabilistic duration
        volatile double duration @unit(s) = default(0s);
        double durationMin @unit(s) = default(0s);
        double durationMax @unit(s) = default(0s);
        
        @display("i=block/delay");
        @class(DelayUnit);
    gates:
        /// The input gate to receive incomming messages to delay.
        input in;
        /// The ouput gate to forward messages after the calculated duration.
        output out;
}
