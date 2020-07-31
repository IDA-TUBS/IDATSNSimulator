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

// The ArbiterIEEE802_1Qbv is a module which is conected to Arbiter module interface 
// The ArbiterIEEE802_1Qbv arbitrate the in BuffersModule stored Frames and send them out
// Frames are selected for transmission based on the IEEE 802_1Qbv.
simple ArbiterIEEE802_1Qbv like Arbiter
{
    parameters:
        ///
        int supportedPriorities = default(8);
        ///
        double periodDuration @unit(ns) = default(100 us);
        ///
        double criticalIntervalDuration @unit(ns) = default(60 us);
        ///
        double timeOffset @unit(ns) = default(0);
        ///
        int criticalPayloadSize = default(1500);
        ///
        int noncriticalPayloadSize = default(1500);
        ///
        string criticalPriorityClasses = default("7 6");
        
        @display("i=block/mac;bgb=513,275");

    gates:
        input in;
        output out;
        inout control;
}