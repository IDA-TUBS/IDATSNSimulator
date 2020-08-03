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

#ifndef SRC_SWITCH_EGRESSPORT_BUFFERSMODULE_H_
#define SRC_SWITCH_EGRESSPORT_BUFFERSMODULE_H_

#include <omnetpp.h>
#include "../../messagetype/EthernetFrame_m.h"
#include "../../messagetype/BufferControlMessage_m.h"
#include "FrameBuffer.h"

using namespace omnetpp;

/**
 * BuffersModule() is part of the EgressPort() as well as the
 * interface Arbiter() and interface Classifier(). The BuffersModule()
 * consists several FrameBuffers. The number of FrameBuffers() in
 * BufferModule() depends on the number of supported priorities.
 * For each priority a FrameBuffer() is created.
 *
 * An incoming Ethernet frame is stored into the FrameBuffer() with
 * the same priority depending on its priority. When an Ethernet
 * frame appears the first time, BuffersModule() sends a control
 * message with status BUFFER_LEVEL_UPDATE to the Arbiter(). The
 * Arbiter() then starts the arbitration and informs via control
 * Message SEND_AUTHORISATION the BuffersModule() which frame to be
 * send.
 */
class BuffersModule : public cSimpleModule
{
public:
    /**
     * Constructor BuffersModule, nothing to do here
     */
    BuffersModule();

    /**
     * Destructor BuffersModule, delete every stored Ethernet frame and
     * every generated FrameBuffer.
     */
    virtual ~BuffersModule();

    /**
     * Called after finishing the simulation. Delete every stored
     * Ethernet frame in the buffer.
     */
    virtual void finish() override;

    /**
     * Initialize BuffersModule. First get the user inputs. Than set up
     * the Buffer. For each priority there is a buffer and a maximum size.
     */
    virtual void initialize() override;

    /**
     * Only external messages to handle. There a two different types
     * of external messages. The first one is an EthernetFrame the
     * second one is a BufferControlMessage from the arbiter.
     */
    virtual void handleMessage(cMessage* msg) override;

private:
    int numberSupportedPriorities;
    std::vector<FrameBuffer*> frameBuffers;
    int maximumBufferSize;

    /**
     * Set the initial user inputs which are made in ini file.
     *
     * To set up the buffers module the inputs from ini-file are taken.
     * The number of supported priorities is taken to set up one FrameBuffer
     * for each priority. The maximum size of the buffers indicates the storage
     * capacity of each buffer.
     */
    void setInitinalUserInputs();


    /**
     * Get the number of supported priorities.
     *
     * Number of priorities is set by user in .ini file during the set up
     * of the network. By default the number is set to 8 priorities with values
     * between 0 and 7. The number is needed to set up the number of buffers.
     * Each priority has its own frame buffer.
     *
     * @returns The integer value of the number of priorities.
     */
    int getNumberSupportedPriorities();


    /**
     * Get the size of the buffer, which indicates the storage capacity
     *
     * The maximum buffer size indicates how many frames can be stored in the buffer
     * depending on the frame size. By default the maximum buffer size is set to
     * 4096 Byte.
     *
     * @returns The integer value of the maximum buffer size in Byte.
     */
    int getMaximumBufferSize();

    /**
     * Set up a frame buffer for each priority.
     *
     * Depending on the number of supported priorities and the maximum buffer
     * size a frame buffer of class FrameBuffer is set up.
     */
    void setUpFrameBuffers();

    /**
     * Deal with an external incoming message
     *
     * BuffersModule has only to deal with external messages. It has to distinguished
     * two types of external messages. The first message type is the EthernetFrame
     * and the second one the BufferControlMessage.
     *
     * @param message Pointer to type cMessage, can be EthernetFrame or BufferControlMessage
     */
    void dealWithExternalMessage(cMessage* message);

    /**
     * Set true if message is a control message and false otherwise.
     *
     * BuffersModule has to deal with two different message types. Message
     * type BufferControlMessage arrives BuffersModule from "control" gate.
     * Message type EthernetFrame arrives BuffersModule from "in" gate. To
     * distinguished the message type check the gate at which the message
     * arrivals.
     *
     * @param message Pointer new arrived cMessage
     * @returns true if message type is BufferControlMessage
     */
    bool isMessageControlMessage(cMessage* message);

    /**
     * Deal with new arrived message as BufferControlMessage.
     *
     * New arrived message is a BufferControlMessage from the arbiter.
     * Cast the cMessage into a BufferControlMessage, get the control instruction
     * from the arbiter and handle this.
     *
     * @param message Pointer to type cMessage, is a BufferControlMessage
     */
    void dealWithBufferControl(cMessage* message);

    /**
     * Deal with new arrived message as EthernetFrame.
     *
     * New arrived message is an EthernetFrame. Cast cMessage into an EthernetFrame.
     * Get the priority and checked whether Ethernet frames with the same priority are
     * already stored in buffer. Store the arrived Ethernet frame in the buffer and
     * trigger an update message to the Arbiter if the buffer is empty.
     *
     * @param message Pointer to type cMessage, is an EthernetFrame
     */
    void dealWithEthernetFrame(cMessage* message);

    /**
     * Cast cMessage into a BufferControlMessage.
     *
     * Incoming external message is cast into a BufferControlMessage. It is also
     * checked if the message exits, otherwise an error is thrown.
     *
     * @param message Pointer to cMessage that will be cast into BufferControlMessage
     * @returns The pointer to the cast BufferControlMessage
     */
    BufferControlMessage* castMessageIntoBufferControlMessage(cMessage* message);

    /**
     * Get the control instructions for BuffersModule from BufferControlMessage
     *
     * BufferControlMessages are messages between arbiter and BuffersModule.
     * There a two instructions from arbiter that the BuffersModule has to handle.
     *
     * @param controlMessage Pointer to BufferControlMessage containing instructions
     * @returns an integer from enum messageInformationConstant indicating the instruction
     *          how to deal with the message
     */
    int getBufferControlInstruction(BufferControlMessage* controlMessage);

    /**
     * Handle the instructions from the BufferControlMessage.
     *
     * Their are two different instruction that the BuffersModule has to handle.
     * The first instruction is BUFFER_CONTROL_REQUEST. The arbiter send a
     * BufferControlMessage to BuffersModule to get the fill level of the
     * buffers. The second instruction is SEND_AUTHORISATION. The BuffersModule
     * gets the rights from the arbiter to send an EthernetFrame.
     *
     * @param instruction An integer value indicating the instruction depending on
     *        messageInformationConstant
     * @param controlMessage Pointer to BufferControlMessage
     */
    void handleBufferControlInstruction(int instruction, BufferControlMessage* controlMessage);

    /**
     * Response of BUFFER_CONTROL_REQUEST from Arbiter
     */
    void bufferControlLevelRequest();

    /**
     * Response of SEND_AUTHORISATION from Arbiter. Contains the authorization
     * to send an Ethernet frame.
     *
     * @param controlMessage
     */
    void sendEthernetFrameFromBuffer(BufferControlMessage* controlMessage);

    /**
     * Sends a control message to Arbiter() when an EthernetFrame with new
     * priority appears.
     *
     * On arrival of an Ethernet frame with a priority that is not yet sorted
     * in a FrameBuffer(), an control message with status BUFFER_LEVEL_UPDATE
     * is transmit to the Arbiter(). Thus the Arbiter() is always kept up
     * to date.
     */
    void triggerUpdateMessageNewPriorityArrival();

    /**
     * Cast cMessage into an EthernetFrame.
     *
     * Incoming external message is cast into an EthernetFrame. It is also checked if
     * the message exits, otherwise an error is thrown.
     *
     * @param message Pointer to cMessage that will be cast into an EthernetFrame
     * @returns The pointer to the cast EthernetFrame
     */
    EthernetFrame* castMessageIntoEthernetFrame(cMessage* message);

    /**
     * Check whether an EthernetFrame with its priority is already stored in the
     * buffer.
     *
     * The arbiter has to be informed for new arriving EthernetFrames. Therefore
     * it is checked whether an EthernetFrame is the first one of its priority.
     * If it is the first one return true and if EthernetFrames with the same
     * priority are already stored in the buffer return false.
     *
     * @param priority Integer value of the EthernetFrame priority
     * @returns true, if no EthernetFrame is stored in the Buffer otherwise false.
     */
    bool isFristArrivingFrameWithPriority(int priority);

    /**
     * Arriving EthernetFrame is stored in buffer with its priority.
     *
     * @param priority Integer value of the EthernetFrame priority
     * @param ethernetFrame Pointer to EthernetFrame that is stored in buffer
     */
    void storeEthernetFrameInBuffer(int priority, EthernetFrame* ethernetFrame);

    /**
     * Set up the information about each buffer for the arbiter.
     *
     * The BuffersModule has to provide information the arbiter with information
     * about each buffer so that the arbiter can decide which Ethernet frame to send.
     * The relevant informations are the priority of the buffer and the frame length
     * of the first frame in the buffer.
     *
     * @returns A vector containing the priority and frame length of first frame in each buffer
     */
    std::vector <ethernetHeaderInformation*> setUpBufferInformation();

    /**
     * Get the information of one buffer.
     *
     * Relevant information are the priority of the buffer and the frame length
     * of the first frame in the buffer. If no frame is stored the frame length
     * is set to 0. This indicates the arbiter that no frame of this priority
     * is stored.
     *
     * @param Integer value of the buffer priority
     * @returns Pointer to struct ethernetHeaderInformation containing the priority and first frame length
     */
    ethernetHeaderInformation* getFrameBufferInformation(int priority);

    /**
     * A BufferControlMessage is sent to the arbiter.
     *
     * BufferControlMessage is sent to the arbiter. The message contains information
     * about each buffer. The priority and the length of the first frame in each buffer
     * is provided for the arbiter. The status of BufferControlMessage depends on the
     * send instruction from arbiter or if a frame with a new priority arrived.
     *
     * @param bufferInformation Vector containing for each buffer priority and frist frame lenght
     * @param status Integer value status of the BufferControlMessage from type messageInformationConstant
     */
    void sendBufferControlMessage(std::vector <ethernetHeaderInformation*> bufferInformation, messageInformationConstant status);


    /**
     * Delete every remaining frame in the buffers after finishing the program
     *
     * After call finish or the class destructor clean up the memory. Every
     * remaining frame in the buffers has to be deleted to avoid warnings and
     * errors.
     */
    void deleteFramesInBuffers();

    /**
     * Delete every remaining FrameBuffer for every priority.
     *
     * After call finish or the class destructor clean up the memory. Every
     * remaining FrameBuffer has to be deleted to avoid warnings and
     * errors.
     */
    void deleteBuffers();
};

Define_Module(BuffersModule);

#endif /* SRC_SWITCH_EGRESSPORT_BUFFERSMODULE_H_ */
