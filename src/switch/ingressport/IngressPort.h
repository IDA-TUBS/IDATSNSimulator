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

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

#include "../../common/EthernetMacros.h"
#include "../../messagetype/EthernetFrame_m.h"


class IngressPort : public cSimpleModule
{
    public:

        /**
         * Constructor IngressPort, nothing to do here
         */
        IngressPort();

        /**
         * Destructor IngressPort
         * Cancel and delete self trigger Messages from type cMessage
         */
        virtual ~IngressPort();

        /**
         *  initialization of variables form ned files
         *  Initialize vector for statistics and set self trigger first time
         */
        void initialize();

        /**
         *  deal with incoming Messages and select between Messages from outside e.g. Ethernet Messages or self Messages
         *
         *  @param message Pointer to type cMessage, can be EthernetFrame or self Messages
         */
        void handleMessage(cMessage* message);

    private:

        int macAddress;
        int ingressPortID;
        simtime_t sampleTimePeriod;
        cMessage* triggerSelf = new cMessage();
        int numBytes;

        cOutVector linkUtilizationStatistics;

        /**
         *  deal with internal self Messages
         *  Take next snapshot for statistic values if Message is triggerSelf
         *
         *  @param message Pointer to type cMessage
         */
        void dealWithInternalMessage(cMessage* message);

        /**
         *  deal with incoming external Messages and check if it is an Ethernet Frame
         *
         *  @param message Pointer of type cMessage
         */
        void dealWithExternalMessage(cMessage* message);

        /**
         * check if incoming Message is an Ethernet Frame and if so return the Frame
         *
         * @param message Pointer to type cMessage
         *
         * @return message Pointer of type EthernetFrame
         */
        EthernetFrame* checkAndCastEthernetFrame(cMessage* message);

        /**
         * Sends out the incoming Ethernet Frame
         *
         * @param message Pointer of type EthernetFrame
         */
        void dealWithEthernetFrame(EthernetFrame* ethernetFrame);

};

Define_Module(IngressPort);
