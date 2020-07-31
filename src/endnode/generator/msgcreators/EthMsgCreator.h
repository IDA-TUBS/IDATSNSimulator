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

#ifndef NODE_EthMsgCreator_H_
#define NODE_EthMsgCreator_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * The EthMsg Creator instantiates and forwards a raw Ethernet packet with a given size after stimulation from the preceding module.
 */
class EthMsgCreator : public cSimpleModule
{

public:
    /**
     * Default constructor.
     */
    EthMsgCreator();

    /**
     * The Destructor
     */
    virtual ~EthMsgCreator();



protected:

    /// Enables detailed statistics of the path an Ethernet message takes
    bool enableFineGrainedLatencyStats;
    /// The frame sequence ID unique for a given globalStreamId
    int64_t uniqueFrameId;

    /**
     * Overridden method, initializes the module parameters
     */
    virtual void initialize() override;

    /**
     * Overridden method, called at the end of the simulation
     */
    virtual void finish() override;

    /**
     * Overridden method, triggered by a traffic source and creates one Ethernet message each time it is called.
     * The message is passed to the Stack and the Ethernet network.
     *
     * @param msg each incoming message is interpreted as a stimulation
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * This method calculates unique IDs for Ethernet messages. It returns the current value and increases it thereafter.
     */
    int64_t getNextUniqueFrameId();
};

Define_Module(EthMsgCreator);

#endif /* NODE_EthMsgCreator_H_ */
