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

#ifndef NODE_SampleMsgCreator_H_
#define NODE_SampleMsgCreator_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * Create and forwards an RTPS sample after stimulation from a source module
 */
class SampleMsgCreator : public cSimpleModule
{

public:
    /**
     * Empty default constructor
     */
    SampleMsgCreator();

    /**
     * Empty default destructor
     */
    virtual ~SampleMsgCreator();

protected:

    /// The frame sequence ID unique for a given globalStreamId
    int64_t sampleSequenceNumberId;

    /**
     * Overwritten method, initializes module after its creation.
     */
    virtual void initialize() override;

    /**
     * Overridden method, triggered by a traffic source and creates one Sample each time it is called. The sample is then passed to the middleware.
     *
     * @param msg Each incoming message is interpreted as a stimulation
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Overridden method, called at the end of the simulation
     */
    virtual void finish() override;

    /**
     * Calculates the unique sequence number of the writer streams samples.
     */
    int64_t getNextSampleSequenceNumberId();

};

Define_Module(SampleMsgCreator);

#endif /* NODE_SampleMsgCreator_H_ */
