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

#ifndef SRC_SWITCH_EGRESSPORT_ARBITERIEEE8023_H_
#define SRC_SWITCH_EGRESSPORT_ARBITERIEEE8023_H_

#include <stdio.h>
#include <fstream>
#include <string.h>
#include <omnetpp.h>
#include <map>
#include <vector>

#include "../../messagetype/BufferControlMessage_m.h"
#include "../../messagetype/EthernetFrame_m.h"
#include "../../common/EthernetMacros.h"

using namespace omnetpp;

class ArbiterIEEE802_3 : public cSimpleModule
{
    public:
        /**
         *  Constructor ArbiterIEEE802.1Q, nothing to do here
         */
        ArbiterIEEE802_3();

        /**
         *  Destructor ArbiterIEEE802.1Q, nothing to do here
         */
        virtual ~ArbiterIEEE802_3();

        /**
         *  Initialize Arbiter and load informations form Parent
         *  Start calculation of transmission speed
         */
        void initialize();

        /**
         *  Classification of incoming messages into internal and external sources
         */
        void handleMessage(cMessage* message);

    private:

        std::vector<ethernetHeaderInformation*> currentBufferStatusList;
        int numberPriorities;
        cModule* parentModul;
        cGate* parentGateOut;
        cChannel* parentChannelOut;
        double channelTransmissionRate;
        bool arbiterIsBusy;
        bool ethernetFrameIsWaiting;
        EthernetFrame* ethernetFrameWaitForSend;
        cMessage* triggerSelf = new cMessage();
        int rememberLastWinner;
        //simtime_t transmissionDelay;

        /**
         *  Deals with internal messages and process timer timeouts.
         *  Timer trigger start of transmission to next router
         *
         *  @param message incoming message
         */
        void dealWithInternalMessage(cMessage* message);

        /**
         *  Deals with external incoming messages and check if messages was external Ethernet Frame or Control Buffer Message
         *
         *  @param message incoming external message
         */
        void dealWithExternalMessage(cMessage* message);

        /**
         *  Process incoming Etherent frames and store them. Only one Frame is allowed.
         *  Start transmission delay process chain
         *
         *  @param frame
         */
        void dealWithEthernetFrame(cMessage* frame);

        /**
         *  Process incoming Control Messages
         *  Check what kind of Control Message is received.
         *  Buffer Level Update or Buffer Level Response.
         *  The Buffer Level Updates are processed if Arbiter is not busy
         *
         *  @param controlMessage
         */
        void dealWithControlMessage(cMessage* controlMessage);

        /**
         *  prepare the ready value of BufferStatusList
         *  caused by frame length
         *  if length = 0 frame is not ready
         *
         *  @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        std::vector<ethernetHeaderInformation*> prepareBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         * This function controls the arbiter and call the steps which are necessary to prepare and arbitrate frames
         *
         * @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        void controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         *  Arbiter which choose the next winning Frame for next transmission out of the Buffer Status List
         *
         *  @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        int arbitrateBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         *  Send Buffer the result of arbitration, send the authorized Buffer to transmit next frame
         *
         *  @param authorizedBuffer
         */
        void sendBufferAuthorizedBuffer(int authorizedBuffer);

        /**
         *  Send a request to Buffer to check the current Buffer status fill level
         */
        void sendBufferRequest();

        /**
         *  Calculate based on Ethernet frame length transmission delay
         *
         *  @param ethernetFrame
         *
         *  @return simtime_t
         */
        simtime_t calculateTransmissionDelay(EthernetFrame* ethernetFrame);

        /**
         *  Calculate Transmission seed of channel
         *  SafeGuard prevent for choose nullpointer as channel
         */
        void calculateTransmissionSpeed();

        /**
         * set the trigger for sending frame out
         *
         * @param transmissionPathDelay
         */
        void settriggerDelayEnd(simtime_t transmissionPathDelay);

        /**
         * send Frame out after Transmission time Delay is past
         */
        void sendFrameOut();

        /**
         *
         */
        void debugStatusInitialization(std::vector<ethernetHeaderInformation*> BufferStatusList);
};

Define_Module(ArbiterIEEE802_3);

#endif /* SRC_SWITCH_EGRESSPORT_ARBITERIEEE8023_H_ */
