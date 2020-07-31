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

#ifndef _RTPS_RTPSTOETHERNETADAPTER_H_
#define _RTPS_RTPSTOETHERNETADAPTER_H_

#include "../../../../common/ValidationHandler.h"
#include "../../../../endnode/middleware/rtps/messages/RTPSEthMessage_m.h"

#include <omnetpp.h>

using namespace omnetpp;

#define _adapter_src_verbose_ if (false)

/**
 * The adapter fills the EthernetFrame parameters to the RTPSEthMessage, from which it is derived.
 */
class RtpsToEthernetAdapter : public cSimpleModule
{

  public:
    /**
     * Default constructor
     */
    RtpsToEthernetAdapter();

    /**
     * Default destructor
     */
    virtual ~RtpsToEthernetAdapter();

    /// Frame sequence ID counter for unique frame identification of the middleware
    unsigned long long frameSequenceId;

  protected:

    /**
     * Put the information from the middleware to configure the Ethernet message.
     *
     * @param packet the outgoing packet to send and to configure for Ethernet transmission.
     */
    void sendRtpsPacket(RTPSEthMessage *packet);

    /**
     * Empty initialization
     */
    virtual void initialize() override;

    /**
     * Initializes the relevant context of the derived EthernetFrame to the packet.
     *
     * @param msg represents the outgoing msg to adapt for EthernetFrame compliance.
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Returns the number of send frames
     */
    virtual void finish() override;

    /**
     * Return and increment the frame sequence ID
     */
    unsigned long long getNextFrameSequenceId();

};

#endif /* _RTPS_RTPSTOETHERNETADAPTER_H_ */
