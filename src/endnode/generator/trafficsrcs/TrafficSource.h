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

#ifndef NODE_GENERATOR_TRAFFICSOURCE_H_
#define NODE_GENERATOR_TRAFFICSOURCE_H_

#include <omnetpp.h>
using namespace omnetpp;

#define _burst_src_verbose_ if (true)

/**
 * Implementation of the stimuli-generating component based on a periodic burst model
 */
class TrafficSource : public cSimpleModule
{
public:
    TrafficSource();
    virtual ~TrafficSource();

protected:
    /**
     * Initialize the module
     */
    virtual void initialize();

    /**
     * Receiving self messages to control the output of source packets
     *
     * @param msg A self message to start a new burst or a single transmission inside of a burst
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * Sends a packet and reduces the bust count.
     * Also activates pending bursts if present.
     */
    void sendPacket();

    /**
     * Creates the next generation message according with the specified parameters.
     * Is only called after interBurstMsg self event.
     * Schedules the next event in a burst.
     *
     * @param offset Is only set the first time this method is called
     */
    void scheduleNextBurst(simtime_t offset);

    /**
     * Starts a burst if no burst is currently active
     */
    void scheduleNextIntraBurst();

private:

    /// Controlls if the generator should be active
    bool generatorEnable;

    /// The global stream Id passed to Ethernet messages if created in the following helper module.
    int globalStreamId;

    /// The intra-burst self event. Signals that a burst is currently scheduled (blocking until next event)
    cMessage* intraBurstMsg;

    /// The inter-burst self event. Signals that the next activation to send out is ready.
    cMessage* interBurstMsg;

    /// The number of pending bursts
    int pendingBursts;

    /// The number of pending activations of the current burst
    int pendingIntraBurstSize;
};

Define_Module(TrafficSource);

#endif /* NODE_GENERATOR_TRAFFICSOURCE_H_ */
