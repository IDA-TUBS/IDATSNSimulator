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

#ifndef SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_WRITER_H_
#define SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_WRITER_H_


#include "../../../../endnode/middleware/rtps/entities/Endpoint.h"
#include "../../../../endnode/middleware/rtps/entities/HistoryEntry.h"
#include "../../../../endnode/middleware/rtps/messages/RTPSEthMessage_m.h"

#include <omnetpp.h>
using namespace omnetpp;

/**
 * The RTPS Writer entity sends generated samples to its subscribed readers based on the streamID routing mechanism implemented in the network.
 */
class Writer : public cSimpleModule, Endpoint
{

  private:

    /// Send event self message
    cMessage *sendEvent;
    /// Self event to trigger the registration of the writer at the participant
    cMessage *registerEvent;

    /// The own MAC address
    int sourceMac;
    /// The MAC address of the remote reader(s)
    int destinationMac;
    /// The own entity ID
    int entityId;
    /// The endity ID of the remote reader(s)
    int destinationEntityId;
    /// The Ethernet Priority indicator
    int streamEthernetPriority;
    /// The writers global stream ID
    int globalStreamId;

    /// Shaping duration for messages of the individual writer
    simtime_t shapingDuration;

    /// The size of the history cache
    int historySizeQoS;
    /// The lifespan duration
    simtime_t lifespan;
    /// Choose best effort or reliable communication
    bool bestEffort;
    /// The fragment size
    int fragmentSize;
    /// The transport priority of the writer
    int transportPriority;

    /// The history cache queue of the writer
    std::list<HistorySampleEntry*> historyCache;
    /// The send queue
    std::list<SampleFragment*> send_queue;

    /// The current sample sequence number
    int currentSampleSequenceNumber;

  public:

    /**
     * The default constructor
     */
    Writer();

    /**
     * The default destructor
     */
    virtual ~Writer();

  protected:

    virtual void initialize() override;
    /**
     * Handles incoming messages, which are Samples from the application (generator), RTPS messages or self messages.
     *
     * @param msg The incoming message.
     */
    virtual void handleMessage(cMessage *msg) override;
    /**
     * Nothing to finish here
     */
    virtual void finish() override;

    /**
     * Register the writer at the participant
     */
    void registerWriter();

    /**
     * Choose the next fragment to send and add it in the send queue
     */
    bool addNextFragmentToSendList();

    /**
     * When this method is called and the sendEvent is not scheduled, than a new fragment is send out if one is pending
     */
    void sendMessage();

    /**
     * Check for all samples if their lifespan have been expired
     */
    void checkLifespanOfSamples();

    /**
     * Create an RTPS message from a sample fragment in order to send it
     *
     * @param sampleFragment The fragment to send
     */
    RTPSEthMessage* createRtpsMsgFromFragment(SampleFragment* sampleFragment);

};

Define_Module(Writer);

#endif /* SRC_ENDNODE_MIDDLEWARE_RTPS_ENTITIES_WRITER_H_ */
