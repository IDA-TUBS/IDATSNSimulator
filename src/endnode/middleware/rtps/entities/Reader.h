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

#ifndef SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_READER_H_
#define SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_READER_H_

#include "../../../../endnode/middleware/rtps/entities/Endpoint.h"
#include "../../../../endnode/middleware/rtps/entities/HistoryEntry.h"
#include "./../../../../common/ValidationHandler.h"

#include <omnetpp.h>
using namespace omnetpp;

/**
 * The RTPS Reader entity subscribes to remote writers and delivers completely received samples to the application/sink
 */
class Reader : public cSimpleModule, Endpoint
{

  private:

    /// Self message to register the reader at the participant
    cMessage *registerEvent;

    /// The size of the history cache
    int historySizeQoS;
    /// Configuration of best effort or reliable communication
    bool bestEffort;
    /// The lifespan of the samples
    simtime_t lifespan;
    /// The deadline of the samples
    simtime_t deadline;
    /// The transport priority QoS parameter
    int transportPriority;

    /// The history cache
    ReaderHistory* readerHistoryCache;

    // Sample latencies of received samples
    /// Histogram for sample Latency from sample generation
    cHistogram SampleLatencySinceGenerationStat;
    /// Vector for sample Latency from sample generation
    cOutVector SampleLatencySinceGenerationStatVec;
    /// Histogram for sample Latency from arrival at writer
    cHistogram SampleLatencySinceWriterArrivalStat;
    /// Vector for sample Latency from arrival at writer
    cOutVector SampleLatencySinceWriterArrivalStatVec;
    /// Histogram for sample Latency from first network injection of the first packet
    cHistogram SampleLatencySinceSampleInjectionStat;
    /// Vector for sample Latency from first network injection of the first packet
    cOutVector SampleLatencySinceSampleInjectionStatVec;
    /// Frame network latency histogram
    cHistogram frameLatencyInjectionToReception;
    /// Frame network latency vector
    cOutVector frameLatencyInjectionToReceptionVec;

    /// The own MAC address
    int sourceMac;
    /// The own endity ID
    int entityId;
    /// The Ethernet Priority
    int streamEthernetPriority;

  public:
    /**
     * The default constructor
     */
    Reader() {}
    /**
     * The default destructor
     */
    virtual ~Reader();

  protected:

    /**
     * Record the histograms here.
     */
    virtual void finish() override;
    /**
     * Initialize all relevant parameters and register the reader at the participant.
     */
    virtual void initialize() override;
    /**
     * Handles incoming messages
     *
     * @param msg Receives RTPSEthMessages and evaluates them.
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Register the reader at the participant for message dispatching.
     */
    void registerReader();


    /**
     * Get the partial history Cache corresponding to the referenced writer
     *
     * @param sourceParticiapntId The ID of the participant related to the writer.
     * @param writerId The ID of the writer itself.
     * @ret Returns the cache corresponding to the specified unique writer.
     */
    std::list<HistorySampleEntry*>*  getCache(int sourceParticipantId, int writerId);

    /**
     * Update the cache after receiving a new RTPS message
     *
     * @param rtpsMsg The received RTPS message to be to update the corresponding cache with.
     * @param historyChache The cache to update.
     */
    void updateCache(RTPSEthMessage* rtpsMsg, std::list<HistorySampleEntry*>* historyCache);

    /**
     * Checks if the received RTPS message contains data from a sample which is not yet stored in the cache
     *
     * @param sequenceNumber The sequence number of the received sample.
     * @param writerEntityId The entity ID of the samples writer.
     * @param sourceParticipantId The ID of the participant the sample writer is placed in.
     * @param historyCache The history Cache related to the samples writer at the reader.
     * @ret True if the sample is relevant.
     */
    bool relevantNewSample(int sequenceNumber, int writerEntityId, int sourceParticipantId,  std::list<HistorySampleEntry*>* historyCache);

    /**
     * Inserts a sample to the correct place in the cache (ordered by the sequence number).
     *
     * @param sample The sample to be placed.
     */
    void insertSampleInCache(HistorySampleEntry* sample);

    /**
     * Check all samples in the cache corresponding to a writer for completion.
     *
     * @param historyCache The history Cache for which all samples are checked for completion.
     */
    void checkCompletion(std::list<HistorySampleEntry*>* historyCache);

    /**
     * Check the lifespan of all samples in the cache corresponding to a writer.
     *
     * @param historyCache The history Cache for which all samples are checked for lifespan expiration.
     */
    void checkLifespan(std::list<HistorySampleEntry*>* historyCache);

    /**
     * Remove samples from the history cache after its maximum size has been exeeded.
     *
     * @param historyCache The history cache to clean from completed, or exceeded entries.
     */
    void cleanHistoryCache(std::list<HistorySampleEntry*>* historyCache);

    /**
     * Creates a sample message from the completed sample Entry and sends it to the sample sink.
     *
     * @param entry The sample entry to convert to a sample message.
     */
    void sendSampleToSink(HistorySampleEntry* entry);

};

Define_Module(Reader);

#endif /* SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_READER_H_ */
