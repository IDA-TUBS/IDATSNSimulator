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

#ifndef RTPS_HISTORYENTRY_H_
#define RTPS_HISTORYENTRY_H_

#include <omnetpp.h>
using namespace omnetpp;

class SampleFragment;
class HistorySampleEntry;

/**
 * This class represents the history cache for the reader, which can subscribe to multiple writer streams
 */
class ReaderHistory
{

  public:

    /// Number of incoming streams
    int nbrStreams;
    /// The maximum history size (maximum number of writers to subscribe)
    int maxNbrStreams;
    /// The cache
    std::list<HistorySampleEntry*> ** cache;

    /**
     * The constructor
     *
     * @ param size Initializes the history with a given size
     */
    ReaderHistory(int size)
    {
        maxNbrStreams = size;
        cache = new std::list<HistorySampleEntry*>*[maxNbrStreams];
        nbrStreams = 0;

    }

    /**
     * The default constructor
     */
    ~ReaderHistory()
    {
        delete cache;
    }

};




/**
 * Represents a sample in the history cache
 */
class HistorySampleEntry
{

  public:

    /// Information on sample granularity
    int sampleSize;
    int sampleSequenceNumber;

    /// The writers global stream ID
    int globalStreamId;

    // Reader and writer context
    int readerEntityId;
    int readerParticipantEntityId;
    int writerEntityId;  // sourceWriterEntityId
    int writerParticipantEntityId; //sourceParticipantId

    /// Fragment information storage
    int numberFragments;
    SampleFragment ** sampleFragmentArray;

    /// Evaluation specific
    bool complete;
    /// Invalidates the entry after the lifespan expired
    bool lifespanExpired;
    /// Invalidates the entry when too many samples are in the cache
    bool historySizeExeeded;
    /// Deadline violation corresponding to latency budget
    bool deadlineViolation;


    /// Statistic related
    /// Sample generation time (in generator)
    simtime_t generationTime;
    /// Sample creation time (arrival in RTPS component)
    simtime_t writerArrivalTime;
    /// Injection time of the first rtps message into the network
    simtime_t writerFirstInjectionTime;
    /// Completely received at the reader
    simtime_t subscriberReceiveTime;
    /// Latencies of the individual rtps_messages
    std::list<simtime_t> rtpsMsgLatencies;


    /**
     * The default constructor
     */
    HistorySampleEntry()
    {
        complete = false;
        lifespanExpired = false;
        historySizeExeeded = false;
        deadlineViolation = false;
        writerFirstInjectionTime = 0.0;
    }

    /**
     * The default destructor
     */
    ~HistorySampleEntry(){}
};


/**
 * Represents a fragment and stores its send/receive state
 */
class SampleFragment{

  public:

    /// The fragment sequence number
    int fragmentSequenceNumber;
    /// The size of the fragment
    int fragmentSize;

    /// Reference to the corresponding sample
    HistorySampleEntry* sampleReference;

    // Fragment status objects
    /// True if the fragment has already been send and has not been negatively acknowledged (writer side)
    bool send;
    /// True if the fragment has been positively acknowledged (writer side)
    bool acked;
    /// True if the fragment has been received (reader side)
    bool received;

    /// The last time the fragment was send by the writer
    simtime_t sendTime;

    /**
     * The default constructor
     */
    SampleFragment()
    {

        // Relevant for the writer and therefore send within the bitmap
        send = false;
        acked = false;
        // Relevant for the Reader
        received = false;

    }

    /**
     * The default destructor
     */
    ~SampleFragment(){}

};


#endif /* RTPS_HISTORYENTRY_H_ */
