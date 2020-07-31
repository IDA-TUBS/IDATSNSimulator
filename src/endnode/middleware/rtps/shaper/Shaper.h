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

#ifndef RTPS_HELPER_SHAPER_H_
#define RTPS_HELPER_SHAPER_H_

#include "../../../../endnode/middleware/rtps/messages/RTPSEthMessage_m.h"

#include <omnetpp.h>
using namespace omnetpp;

/*
 * Shapes messages in case that there are multiple writers and or readers (shaping on participant granularity).
 */
class Shaper : public cSimpleModule
{
  private:

    /// Enable/Disable the shaper
    bool shaperEnabled;
    /// Duration of shaping, i.e. the time between two consecutive shaped messages
    simtime_t shapingDuration;
    // Self-message, which is scheduled between two consecutive shaped messages. Activates the next message to send.
    cMessage *nextMsgReady;
    // Packet queue of messages to be shaped
    std::list<RTPSEthMessage*> rtpsMsgQueue;


  public:
    /**
     * Default constructor
     */
    Shaper(){}
    /**
     * Default destructor
     */
    virtual ~Shaper();

  protected:

    /**
     * Send the message from the queue
     */
    void send_from_queue();

    virtual void finish() override;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};


#endif /* RTPS_HELPER_SHAPER_H_ */
