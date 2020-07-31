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

message EthernetFrame
{
	int frameSize;      //size of the frame in bytes
    int payloadSize;    //size of the payload (including padding bits, which are necessary when the payload is too short)
    int packetSize;     //frame + sync/ifg
    int MACsource;      //MAC address of the source interface unit
    int MACdest;        //MAC address of the destination interface unit
    int priority;       //global priority of the stream, defining the traffic class in AVB/TSN/*02.1Q, is not unique! 
    int streamID;       // A global ID that identifies the application
    bool isMulticast;   // if switched to true a multcast message otherwie singlecast
    bool isBroadcast;   // is this a broadcast message? (if so, the destination MAC should be -1)
    int64_t frameSequenceID; // The frame sequence ID. It is unique for a given global ID
    simtime_t injectionTime;
}
