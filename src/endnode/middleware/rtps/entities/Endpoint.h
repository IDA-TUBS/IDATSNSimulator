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

#ifndef RTPS_ENTITIES_ENDPOINT_H_
#define RTPS_ENTITIES_ENDPOINT_H_

#include "./../messages/RTPSEthMessage_m.h"

#include <omnetpp.h>
using namespace omnetpp;

/**
 * Superclass of RTPS writers and readers. Implements basic functionalities used by both.
 */
class Endpoint{

public:

    /// The unique entity Id
    int entityId;
    /// The entity kind
    int entityKind;
    /// RTPS message sequence ID counter for unique frame identification of RTPS messages regarding one entity
    unsigned long long rtpsMsgSequenceId;

    /**
     * The default constructor
     */
    Endpoint(){}

    /*
     * The default destructor
     */
    ~Endpoint(){}

    /**
     * Calulates the rtps message size of a message and sets the corresponding parameters
     *
     * @param rtpsMsg Represents the relevant message to calculate the size for.
     */
    void calculateRtpsMsgSize(RTPSEthMessage* rtpsMsg);

    /**
     * Return and increment next unique RTPS message ID
     */
    unsigned long long getNextUniqueRtpsMsgId();
};


#endif /* RTPS_ENTITIES_ENDPOINT_H_ */
