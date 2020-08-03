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

#ifndef SWITCH_EGRESSPORT_ARBITERAVB_H_
#define SWITCH_EGRESSPORT_ARBITERAVB_H_

#include <omnetpp.h>;

#include "../../common/ConfigurationReader.h"
#include "../../messagetype/BufferControlMessage_m.h";
#include "../../messagetype/EthernetFrame_m.h";
#include "../../common/EthernetMacros.h"

using namespace omnetpp;

/**
 * Enumeration for Position in AVB configuration File
 */
enum avbTablePostition
{
    SWITCH_MAC = 0,
    EGRESS_PORT_ID = 1,
    PRIORITY = 2,
    SEND_SLOPE = 3,
    IDLE_SLOPE = 4
};

/**
 * Enumerations for Status of Arbiter Classes
 */
enum arbiterClassStatus
{
    CLASS_IDLE = 0,        //nothing to send, no credit stored/to recharge
    CLASS_SENDING = 1,     //currently sending, credit will be modified once the sending is done
    CLASS_WAITING = 2,     //currently waiting for the credit recharge, blocked by shaper
    CLASS_BLOCKED = 3,     //credit available, but another frame (diff. priority) is currently sending, credit going positive
    CLASS_NOTSHAPED = 4
};

/**
 * General Struct for Information about Arbiter Classes
 */
struct arbiterAVBClass
{
    int classCreditValue;
    bool classFrameWaiting;
    bool classIsShaped;
    int classStatus;
    double sendSlope;    //how fast is credit depleated
    double idleSlope;    //how fast is credit recharged?
    simtime_t lastArbitrationTimeStamp;
    bool timerIsScheduled;
};

/**
 * ArbiterAVB is an arbiter which arbitrate the stored Frames in BuffersModule.
 * Arbiter will select based on the CBS algorithm the next sending Frame and store it for the length of the transmission delay
 */
class ArbiterAVB : public cSimpleModule
{
    public:

        /**
        *  Constructor ArbiterAVB, nothing to do here
        */
        ArbiterAVB();

        /**
         *  Destructor ArbiterAVB, nothing to do here
         */
        virtual ~ArbiterAVB();

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
        int egressPortIndex;
        int macAddress;

        configVector avbTableVector;
        cOutVector creditValueStatisticStreamA;
        cOutVector creditValueStatisticStreamB;

        csvVector config;
        std::string configFile;

        bool arbiterIsBusy;
        bool ethernetFrameIsWaiting;
        EthernetFrame* ethernetFrameWaitForSend;
        cMessage* triggerSelf = new cMessage();
        std::vector<arbiterAVBClass*> classInformations;

        /**
         *  Calculate Transmission seed of channel
         *  SafeGuard prevent for choose nullpointer as channel
         */
        void calculateTransmissionSpeed();

        /**
         *  Calculate Transmission seed of channel
         *  SafeGuard prevent for choose nullpointer as channel
         */
        void initializeAVBClassInformation();

        /**
         *  load Class information from AVB configuration file
         */
        void loadAVBClassInformationFromFile();

        /**
         *  Deals with internal messages and process timer timeouts.
         *  Timer trigger start of transmission to next router
         *
         *  @param message incoming message
         */
        void dealWithInternalMessage(cMessage* message);

        /**
         *  deal with incoming external Messages like Control Messages or Ethernet Frames
         *
         *  @param message incomming external message
         */
        void dealWithExternalMessage(cMessage* message);

        /**
         * check if message is an Ethernet Frame and return it
         *
         * @param message this is a comment about the input variable
         *
         * @return pointer to EthernetFrame
         */
        EthernetFrame* checkAndCastEthernetFrame(cMessage* message);

        /**
         * check if message is an BufferControlMessage and return it
         *
         * @param message
         *
         * @return BufferControlMessage*
         */
        BufferControlMessage* checkAndCastBufferControlMessage(cMessage* message);

        /**
         *  deal with incoming Ethernet Frames
         *
         *  @param message
         */
        void dealWithEthernetFrame(cMessage* message);

        /**
         * Select which type of Control Message is received
         *
         * @param controlMessage
         */
        void dealWithControlMessage(cMessage* controlMessage);

        /**
         *  control the arbitration steps. First prepare the bufferList and calculate the credits(recharging if nessecary). Based on new credit value arbitrate and collect statistics.
         *  Last but not least the arbitration result is send to BuffersMoudles.
         *
         *  @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        void controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         *  checks if new Frames arrived in BufferModule
         *
         *  @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        void prepareClassInformations(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         *  choose for which priority the credits have to be updated
         */
        void calculateClassInformationCredits();

        /**
         *  calculate the total credit values of that priority
         *
         *  @param priority
         */
        void calculateCredits(int priority);

        /**
         *  calculate the current credit value based on the time difference of sending the frame
         *
         *  @param priority
         */
        int calculateCreditValues(int priority);

        /**
         *  Arbiter which choose out of the Buffer Status List the current winner for next transmission
         *
         *  @param BufferStatusList is std::vector<ethernetHeaderInformation*>
         */
        std::vector<ethernetHeaderInformation*> prepareBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         *  Arbiter which choose out of the Buffer Status List the current winner for next transmission
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
         *  Send a Request for Buffer to answer the current Buffer Status
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
         * start recharge calculation of each priority with negative credit value
         */
        void setRechargeTime();
        /**
         * set the trigger for sending frame out
         *
         * @param transmissionPathDelay
         */
        void settriggerDelayEnd(simtime_t transmissionPathDelay);

        /**
         * set the trigger for credit recharge of one priotity
         *
         * @param calculatedRechargeTime, priority
         */
        void settriggerCreditRecharged(simtime_t calculatedRechargeTime, int priority);

        /**
         * calculated recharge Time
         *
         * @param priority
         *
         * @return simtime_t
         */
        simtime_t calculatedRechargeTime(int priority);

        /**
         * send Frame out after Transmission time Delay is past
         */
        void sendFrameOut();

        /**
         * update class information status of each priority
         *
         * @param authorizedBuffer
         */
        void updateClassInformationsStatus(int authorizedBuffer);

        /**
         * This function record Credit Values to cOutVector structures
         * If more than two Priorities exist AVB will work and than statistics are collected.
         * Just Stream A and Stream B are collected, BE is ignored
         *
         * @param BufferStatusList with informations about AVB Arbiter
         */
        void collectCreditsStatistics(std::vector<ethernetHeaderInformation*> BufferStatusList);

        /**
         * shows the credit values and current status of priorities
         */
        void debugStatus();

        /**
         * shows the status of Arbiter initializations class
         */
        void debugStatusInitialization();

};

Define_Module(ArbiterAVB);

#endif /* SWITCH_EGRESSPORT_ARBITERAVB_H_ */
