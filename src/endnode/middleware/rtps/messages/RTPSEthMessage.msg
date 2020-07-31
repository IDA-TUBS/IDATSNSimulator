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

cplusplus {{
#include "../../../../messagetype/EthernetFrame_m.h"
}}

class EthernetFrame;

//
// RTPS message specification send over Ethernet
//
message RTPSEthMessage extends EthernetFrame
{
    // Calculated values
    int payloadSize;
    int protocolOverhead;
    int rtpsMessageSize;
    
    //  Locator information
    int sourceMac;
    int destinationMac;
    
    // Timing info on sample granularity
    /// Generation time in the generator
    simtime_t generationTime;
    /// Reception time at the writer
    simtime_t writerArrivalTime;
    /// First fragment send at the writer
    simtime_t writerFirstInjectionTime;
    /// Completely received at the reader
    simtime_t subscriberReceiveTime;
    
    // Other evaluation data
    // The unique stream ID of a writer
    int globalStreamId;
    // The unique rtps message ID of an entity
    int64_t  rtpsMsgSequenceId;

    // QoS context
    int transportPriority;
    int ethernetPriority;
    int generalFragmentSize;
       
    //
    // RTPS message content
    //
    
    // Header
    int sourceNodeId; // GUID part 1
    int sourceParticipantId; // GUID part 2
        
    // AckNack
    bool ackNackIsSet;
    int readerId;
    int writerId;
    int  readerSNStateBase;
    int  readerSNStateNbrBits;
    bool readerSNStateBitmap[256];
        
    // Data
    bool dataIsSet;
    // readerId (s.o.)
    // writerId (s.o.)
    int writerSN;
    int sampleSize;
    
    // DataFrag
    bool dataFragIsSet;
    // readerId (s.o.)
    // writerId (s.o.)
    // writerSN (s.o.)
    int fragmentStartingNum;
    int fragmentsInSubmessage;
    int fragmentSize;
    // sampleSize (s.o.)
    
    
    // HeartBeat
    bool heartBeatIsSet;
    // readerId (s.o.)
    // writerId (s.o.)
    int firstSN;
    int lastSN;
    

    // HeartBeatFrag
    bool heartBeatFragIsSet;
    // readerId (s.o.)
    // writerId (s.o.)
    
    // InfoDestination
    bool infoDestinationIsSet;
    int destinationNodeId; // GUID part 1
    int destinationParticipantId; // GUID part 2
    
    // InfoTimestamp
    bool infoTimestampIsSet;
    simtime_t timestamp;
    
    // NackFrag
    bool nackFragIsSet;
    // readerId (s.o.)
    // writerId (s.o.)
    // writerSN (s.o.)    
    int  fragmentNumberStateBase;
    int  fragmentNumberStateNbrBits;
    bool fragmentNumberBitmap[256];
    
}
