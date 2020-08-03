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

#ifndef SRC_ENDNODE_SINK_SAMPLESINK_H_
#define SRC_ENDNODE_SINK_SAMPLESINK_H_

#include <stdio.h>
#include <string.h>
#include <map>

#include "../../messagetype/EthernetFrame_m.h"
#include "../../common/ValidationHandler.h"
#include "../../endnode/middleware/rtps/messages/Sample_m.h"
#include "../../statistics/StatisticManager.h"

#include <omnetpp.h>
using namespace omnetpp;

#define _eth_sampleSink_verbose_ if(true)

/**
 * SampleSink that gathers statistics for incoming samples.
 */
class SampleSink : public cSimpleModule
{
protected:

    virtual void initialize() override;
    /**
     * Handles incoming messages
     *
     * @param msg The incoming sample
     */
    virtual void handleMessage(cMessage *msg) override;

private:

    /// One MAC Address of the Endpoint
    int myMac;

};

#endif
