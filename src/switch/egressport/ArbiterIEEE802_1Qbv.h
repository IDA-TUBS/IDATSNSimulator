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

#ifndef SRC_SWITCH_EGRESSPORT_ARBITERIEEE802_1QBV_H_
#define SRC_SWITCH_EGRESSPORT_ARBITERIEEE802_1QBV_H_

#include <omnetpp.h>
#include <stdio.h>
#include <fstream>
#include <unordered_set>
#include <map>

#include "../../messagetype/BufferControlMessage_m.h"
#include "../../messagetype/EthernetFrame_m.h"
#include "../../common/EthernetMacros.h"
#include "../../statistics/StatisticManager.h"

/**
 * Enum of TAS time segments
 */
enum timeSegments
{
    CRITICAL_SENDING      = 1,
    CRITICAL_GUARDBAND    = 2,
    NONCRITICAL_SENDING   = 3,
    NONCRITICAL_GUARDBAND = 4,
};

/**
 * ArbiterIEEE802_1Qbv is an Arbiter() which arbitrate the stored Ethernet frames
 * in BuffersModule. It is also known as Time Aware Shaper (TAS).
 *
 * TAS is a circuit-switched arbitration. The period is divided into two intervals
 *     1. critical interval
 *     2. noncritical interval
 * Each interval is also divided into the following segments:
 *     *.1 sending segment
 *     *.2 guard band segment
 * Only Ethernet frames with critical priorities are sent in the critical interval
 * of the sending segment.
 * In IEEE802_1Qbv, traffic classes with priorities 6 and 7 are classified as
 * critical (set as default). For simulation purpose it is also possible to set
 * other classes as critical as well. The guard band segment is only as long
 * for an Ethernet frame to complete its transmission. No Ethernet frames start
 * their transmission in this segment. In noncritical sending segment only
 * non critical priority classes are sent (default priority 5-0).
 */
class ArbiterIEEE802_1Qbv : public cSimpleModule
{
public:

    /**
     * Class constructor - nothing to do here
     */
    ArbiterIEEE802_1Qbv();


    /**
     * Class destructor
     *
     * Cancel and delete the self messages triggerSelf and segmentTriggerTimerMessage.
     */
    virtual ~ArbiterIEEE802_1Qbv();


    /**
     * Initialize the time aware shaper (TAS).
     *
     * For the initialization: Get the period and critical interval
     * duration. Get the maximum payload size of critical and noncritical
     * Ethernet frames. Calculate the guard band and sending segments. Set up
     * the segmentTriggerTimerMessage to simulate the time states of TAS. Get
     * the current state and the next state of TAS.
     */
    virtual void initialize(void) override;


    /**
     * Handle arriving messages - internal and external messages
     * 
     * There are two types of messages to handle. Internal messages are self messages
     * that simulates time triggers. TAS two time trigger are required. The first one
     * is the segmentTriggerTimerMessage. It is always triggered when the duration of
     * a time segment is up. The second one is triggerSelf. It simulates the transmission
     * delay from an Ethernet frame.
     * There are also two types of external messages to handle. An arriving Ethernet frame
     * is temporarily stored until its transmission delay is up. Than it is transmit to
     * the next module. A BufferControlMessage contains information about the fill level
     * of the FrameBuffers() in BufferModule().
     */
    virtual void handleMessage(cMessage* message) override;

private:
    /// the state is equal the current time segment
    int currentState;
    /// the next state is equal to the next time segment after the current
    int nextState;

    /// set by user -> defines the maximum payload number in bytes for critical priorities frames
    int maximumCriticalPayloadSize;
    /// set by user -> defines the maximum payload number in bytes for noncritical priorities frames
    int maximumNoncriticalPayloadSize;

    /// number of supported priorities (= default 8 priorities)
    int numberSupportedPriorities;
    /// MAC address of the switch
    int switchMAC;

    /// index of the EgressPort via which the frame is forwarded
    int portIndex;

    /// set by user -> defines the duration of the period (= default 5 ms)
    simtime_t periodDuration;
    /// set by user -> defines the duration of the critical interval
    simtime_t criticalIntervalDuration;
    /// critical interval is split into sending and guard band segment
    simtime_t criticalSendingSegmentDuration;
    /// guard band segment is as long as a critical frame needs for transmission
    simtime_t criticalGuardBandSegmentDuration;
    /// non critical interval duration is the time remaining from the subtraction of period and criticalInterval
    simtime_t noncriticalIntervalDuration;
    /// non critical interval duration is split into sending and guard band segment
    simtime_t noncriticalSendingSegmentDuration;
    /// non critical guard band segment is as long as a non critical frame needs for transmission
    simtime_t noncriticalGuardBandSegmentDuration;

    /// integer vector which stores the critical priorities (= default ("7 6"))
    std::vector<int> criticalPriorityClasses;

    /// timer, which allows to start the simulation in any time segment (= default 0 -> critical timing segment)
    simtime_t timeOffset;
    /// timer to trigger a a self message
    simtime_t triggerTimer;

    /// cMessage timer for changing states (time segments)
    cMessage* segmentTriggerTimerMessage = new cMessage();
    /// cMessage timer for trigger Ethernet frame transmission
    cMessage* triggerSelf = new cMessage();

    /// arbiter stores only one Ethernet frame which must wait for its transmission to simulate transmission rate
    EthernetFrame* ethernetFrameWaitForTransmission;

    ///
    std::vector<ethernetHeaderInformation*> currentBufferStatusList;

    /// boolean vector indicating which of the priorities is critical (= true) or not (= false)
    std::vector<bool> isCritical;
    /// variable that indicates that the arbitration is already in progress
    bool arbiterIsBusy;

    bool frameStoredInBuffer;

    /**
     * Get the duration of the period
     *
     * The period duration is set by the user in .ini file and defines the
     * the time length of a single arbitration run. Per default it is set to
     * 5 ms.
     *
     * @returns the duration of the period
     */
    simtime_t getPeriodDuration();


    /**
     * Get the duration of the critical interval
     *
     * The period is divided into critical and noncritical interval. The
     * duration of the critical interval has to be set by the user. Per default
     * it is set to 3 ms.
     *
     * @returns the duration of the critical interval
     */
    simtime_t getCriticalIntervalDuration();


    /**
     * Convert a double value into simtime_t
     *
     * Time values are entered as double values. Double has to be converted
     * into time values to work with simttime_t.
     *
     * @param valueToBeConverted a double value which has to be converted
     * @returns converted simtime_t value
     */
    simtime_t convertDoubleIntoTime(double valueToBeConverted);


    /**
     * Calculate the duration of the guard band depending on the payload size
     * of an Ethernet frame.
     *
     * The period is divided into critical and noncritical interval. Each interval
     * itself is also divided into sending and guard band segments. The duration
     * of the guard band segment depends on the maximum payload size of the
     * Ethernet frame. The guard band is so long that one Ethernet frame can
     * complete its transmission. But: no frame starts transmission in the guard
     * band!
     *
     * @param payload integer of the Ethernet frame payload size
     * @returns the duration of the guard band
     */
    simtime_t calculateGuardBandDurationFromPayloadSize(int payload);


    /**
     * Calculate the Port speed and get the value.
     *
     * For calculating the duration of the guard band the port speed is required.
     * The guard band is as long as the transmission of an Ethernet frame. The
     * Transmission is calculated by packetSize (in bit) divided by port speed.
     *
     * @returns the port speed
     */
    long getPortSpeed();


    /**
     * Set the packet size of an Ethernet frame.
     *
     * From the payload the packet size of an Ethernet frame is set. The frame
     * overhead (22 byte), packet overhead (8 byte) and interpacket gap (12 byte)
     * are added to the payload. It is required for calculating the transmission
     * duration of an Ethernet frame.
     *
     * @return the packet size of an Ethernet frame in byte depending on the payload
     */
    int setEthPacketSizeFromPayload(int payload);


    /**
     * Get the critical priority classes.
     *
     * To transmit Ethernet frames in critical sending segment critical class
     * priorities has to be set. Per default 7 and 6 are critical priorities.
     * They are set as string in .ini file. They are converted as integer and
     * stored into a integer vector.
     *
     * @returns an integer vector containing the critical priorities
     */
    std::vector<int> getCriticalClasses();


    /**
     * Set up a boolean vector for all supported priorities to indicate their criticality.
     *
     * For arbitration it is essential to know which priority is critical and
     * which is not. Therefore a boolean vector is implemented. It contains
     * for each priority an entry. True indicates that the priority is critical
     * and false that it is noncritical.
     *
     * @param criticalPriorities integer vector containing the critical priorities
     * @returns boolean vector indicating which of the priorities is critical (= true) or not (= false)
     */
    std::vector<bool> setUpVectorCriticalPriorities(std::vector<int> criticalPriorities);


    /**
     * Gets the time offset.
     *
     * If the time offset is not set, the arbitration always starts in the
     * critical sending segment. To start in another segment the offset is used.
     * This can be set larger than the period but does not change the timing
     * of the arbitration. Arbitration always starts at time 0.
     *
     * @returns the time offset
     */
    simtime_t getTimeOffset();


    /**
     * Calculates the time remaining after dividing two times.
     *
     * This is required for setting the initial time segment with which the
     * arbitration. It is not possible to divide simtime_t values and get the
     * remainder. Therefore the denominator is subtracted from numerator until
     * denominator is larger than numerator. The rest is returned.
     *
     * @param numeratorTime simtime_t which is to divided
     * @param denominatorTime simtime_t to be divided with
     * @returns the rest of the division
     */
    simtime_t calculateTimeModulo(simtime_t numeratorTime, simtime_t denominatorTime);


    /**
     * Get the state (time segment) with which the arbitration starts.
     *
     * Depending on the time offset the initial state is set. If no time offset
     * state is set (=0) the initial state is always the critical sending segment.
     *
     * @returns integer value of the initial state.
     */
    int getInitialStateAndCalculateTriggerTimer(simtime_t offset);


    /**
     * Get the next state of the current state.
     *
     * There are four states. The state transitions are fixed. From
     * Critical sending to critical guard band to noncritical sending to
     * noncritical guard band back to critical sending. For a correct
     * arbitration, the next state must be set from the current state.
     *
     * @param currentState integer value of the current state (time segment)
     * @returns integer value of the next state (time segment)
     */
    int getNextState(int currentState);


    /**
     * Initialize the timer message segmentTriggerTimerMessage to control the
     * Arbitration.
     *
     * To change the state under correct timing conditions a self message
     * segmentTriggerTimerMessage is initialized. It is always sent, if the
     * duration of a segment is finished.
     */
    void initializeTimerMessage();


    /**
     * Print all initialization configuartions.
     *
     * Print the period duration, the critical interval duaration, non critical
     * interval duration, the critical sending and guard band segment duration
     * as well as the noncritical sending and guard band segment duration. It
     * is also the initial state and the current state printed.
     */
    void printInitializationConfiguration();


    /**
     * Deal with internal messages (self messages).
     *
     * The arbiter has to deal with internal (self) messages and external
     * (bufferControl and Ethernet frame) messages. The internal messages simulates
     * timer functions. Two internal messages must be distinguished. The
     * segmentTriggerTimerMessage triggers always a self message after changing
     * the state. The triggerSelf self message simulates the transmission of
     * an Ethernet frame. It is triggered when the transmission time has expired
     * and the Ethernet frame is transmit.
     *
     * @param message pointer to a self message, segmentTriggerTimerMessage or triggerSelf
     */
    void dealWithInternalMessage(cMessage* message);


    /**
     * Deal with internal message segmentTriggerTimerMessage.
     *
     * A segmentTriggerTimerMessage is arrived. It indicates the finishing of a
     * time segment. The current state is updated as well as the next state. The
     * segmentTriggerTimerMessage is updated and scheduled.
     */
    void dealWithSegmentTimerMessage();

    /**
     * Get the duration time of the segment from the current state
     *
     * To schedule the segmentTriggerTimerMessage the duration of the current
     * state is required.
     *
     * @param state integer value of the state from which the duration time is required
     * @returns the time of the state (time segment)
     */
    simtime_t getSegmentDurationFromCurrentState(int state);


    /**
     * Deal with internal message triggerSelf.
     *
     * A triggerSelf message is arrived. It simulates the transmission time
     * of the Ethernet frame. Its arriving indicates that the transmission time
     * of the stored Ethernet Frame is finished.
     */
    void dealWithTriggerSelf();

    /**
     * Transmit the Ethernet frames that waits for its transmission
     *
     * A triggerSelf message is arrived. It indicates that the transmission time
     * of the Ethernet frame is finished. Now the frame is sent.
     */
    void sendFrameOut();

    /**
     * Send a request to the buffer to get information about stored Ethernet
     * frames in buffer.
     *
     * After finishing the transmission of the waiting Ethernet frame a new
     * Ethernet frame can start its transmission. To get the information about
     * stored Ethernet frames in the buffer a BufferControlMessage with the flag
     * BUFFER_CONTROL_REQUEST is transmit to the BuffersModule().
     */
    void sendBufferRequest();


    /**
     * Deal with external message (Ethernet frame and BufferControlMessage)
     *
     * The arbiter has to deal with internal (self) messages and external
     * (BufferControlMessage and Ethernet frame) messages. BufferControlMessages
     * are messages to control the BuffersModule(). The arriving Ethernet frame
     * is the message that was selected by the arbitration and is now forwarded.
     *
     * @param message cMessage pointer to arriving external message
     */
    void dealWithExternalMessage(cMessage* message);


    /**
     * Deal with the external message BufferControlMessage.
     *
     * Arriving external message is a BufferControlMessage. BufferControlMessages
     * are messages to control the BuffersModule(). There are two different flags
     * that the Arbiter() must handle. BUFFER_CONTROL_UPDATE is a update message.
     * It always arrives if a Ethernet frame with new priority is stored into
     * a FrameBuffer(). BUFFER_LEVEL_RESPONSE is the answer of the
     * BUFFER_LEVEL_REQUEST. First the Arbiter sends a request to the BuffersModule()
     * to get the fill level of the FrameBuffers(). Than the BuffersModule answers
     * with the BUFFER_LEVEL_RESPONSE containing a ethernetHeaderInformation
     * vector.
     *
     * @param controlMessage cMessage pointer to a BufferControlMessage
     */
    void dealWithControlMessage(cMessage* controlMessage);


    /**
     * Control the arbitration of the received ethernetHeaderInformation vector
     * from the BufferControlMessage.
     *
     * With the received ethernetHeaderInformation vector the arbitration phase starts.
     * The ethernetHeaderInformation vector contains for each FrameBuffer in
     * BuffersModule the payload of the first Ethernet frame in the FrameBuffer
     * as well as the priority of the the corresponding buffer. The arbitration
     * phase is divided in two parts: prepareBufferStatusList(...) and
     * arbitrateBufferStatusList
     *
     * @param bufferStatusList a vector containing pointer ethernetHeaderInformation
     */
    void controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList);


    /**
     * Preparation of the ethernetHeaderInformation vector for arbitration.
     *
     * EthernetHeaderInformation is a struct containing the priority of a FrameBuffer()
     * and the payload size of the first Ethernet frame in a FrameBuffer(). If no Ethernet
     * frame is stored in a FrameBuffer() it is indicated with a payload size 0.
     * If an Ethernet frame is in the buffer (payload size > 0), the boolean variable
     * is ready from EthernetHeaderInformation is set true.
     *
     * @param bufferStatusList a vector containing pointer ethernetHeaderInformation
     * @returns a vector containing pointer ethernetHeaderInformation with set boolean
     *          variable ready
     */
    std::vector<ethernetHeaderInformation*> prepareBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList);


    /**
     * Arbitrate the ethernetHeaderInformation vector.
     *
     * After the preparation of the ethernetHeaderInformation vector an Ethernet
     * frame is selected for transmission. It is first checked at which state
     * (time segment) the simulation is. Critical sending segment -> only Ethernet
     * frames with critical priorities are transmit. Critical guard band segment
     * -> no Ethernet frame are transmit. Noncritical sending segment -> only
     * noncritical Ethernet frames (best effort) are transmit. If several messages
     * of different noncritical or critical priorities are in the FrameBuffers,
     * the selection is made according to static priority non preemptive.
     *
     * @param bufferStautsList a vector containing pointer ethernetHeaderInformation
     *                         with set boolean variable ready
     * @returns integer value of the priority which specifies the FrameBuffer from which the
     *          first Ethernet frame is forwarded
     */
    int arbitrateBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList);


    /**
     * Send a authorization BufferControlMessage to BuffersModule.
     *
     * By arbitration the priority of the FrameBuffer is selected that transmit
     * its Ethernet frame. This is told to the BuffersModule() via
     * BufferControlMessage with the flag SEND_AUTHORISATION.
     *
     * @param authorizedBuffer integer value of the FrameBuffers priority
     */
    void sendBufferAuthorizedBuffer(int authorizedBuffer);


    /**
     * Deal with external message Ethernet frame.
     *
     * Arriving external message is an Ethernet frame. The frame is temporarily
     * stored in variable ethernetFrameWaitForTransmission until the
     * transmission time is over. For simulation the transmission time a
     * timer message TriggerSelf is scheduled.
     *
     *
     * @param frame cMessage pointer of an Ethernet Frame
     */
    void dealWithEthernetFrame(cMessage* frame);


    /**
     * Calculate the transmission time of the Ethernet frame.
     *
     * To set up a timer message that simulates the transmission time of the
     * Ethernet frame it must first be calculated. This requires the Ethernet
     * frame payload length and the port speed. (Port speed is obtained from
     * getPortSpeed()).
     *
     * @param ethernetFrame pointer of an EthernetFrame message
     * @returns the transmission time of the Ethernet frame
     */
    simtime_t calculateTransmissionDelay(EthernetFrame* ethernetFrame);


    /**
     * Set up timer message TriggerSelf
     *
     * To simulate the transmission time of the Ethernet frame an self message
     * TriggerSelf is set up. It is scheduledAt the above calculated
     * transmission time.
     *
     * @param transmissionPathDelay the transmission time of the Ethernet frame from
     *                              one module to the next hop
     */
    void setTriggerDelayEnd(simtime_t transmissionPathDelay);

};

Define_Module(ArbiterIEEE802_1Qbv);

#endif /* SRC_SWITCH_EGRESSPORT_ARBITERIEEE802_1QBV_H_ */
