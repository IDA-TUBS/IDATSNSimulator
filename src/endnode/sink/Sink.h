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

#ifndef __sink_H_
#define __sink_H_

#include <omnetpp.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <map>

#include "../../common/ValidationHandler.h"
#include "../../messagetype/EthernetFrame_m.h"
#include "../../statistics/StatisticManager.h"



using namespace omnetpp;

/**
 * Sink module for the Ethernet frames (which are send directly without the DDS)
 * Mainly used to gathers statistics.
 */
class Sink : public cSimpleModule
{
protected:
    /**
     * Constructor
     */
    virtual void initialize() override;
    /**
     * Casts the msg to the EthernetFrame
     * and processes it.
     * @param msg
     */
    virtual void handleMessage(cMessage *msg) override;


private:
    /// MAC address of the sink
    int  macAddress;
    /// counter used to track lost frames
    std::map<int, int> lostFrameCounters;
    /// here we check whether all data was delivered
    std::map<int, int> trackMissingDataMap;

    /// check if frame arrived to the correct destination based on MAC
    virtual bool checkDestinationsMAC(EthernetFrame* ethMsg);
    /// gather end2end latency and return error if latency is negative
    void processEthernetFrameAtDestination(EthernetFrame* ethMsg);
    /// check if there are lost frames evntl. gather statistics
    void checkForMissingFrame(EthernetFrame* ethMsg);

    /// finishing statistics gathering
    void finish();
};

Define_Module(Sink);
#endif
