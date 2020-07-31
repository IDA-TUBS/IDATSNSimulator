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

#include "ArbiterIEEE802_1Qbv.h"

ArbiterIEEE802_1Qbv::ArbiterIEEE802_1Qbv()
{
    // nothing to do here
}

ArbiterIEEE802_1Qbv::~ArbiterIEEE802_1Qbv()
{
    if(this->segmentTriggerTimerMessage)
    {
        cancelAndDelete(this->segmentTriggerTimerMessage);
    }

    if(this->triggerSelf)
    {
        cancelAndDelete(this->triggerSelf);
    }
}

/* -------- INITIALIZATION -------- */
void ArbiterIEEE802_1Qbv::initialize()
{
    // get time values set by user
    this->periodDuration = getPeriodDuration();
    this->criticalIntervalDuration = getCriticalIntervalDuration();
    this->noncriticalIntervalDuration = this->periodDuration - this->criticalIntervalDuration;

    // get maximum payload size critical and noncritical
    this->maximumCriticalPayloadSize = this->par("criticalPayloadSize");
    this->maximumNoncriticalPayloadSize = this->par("noncriticalPayloadSize");

    // calculate guard band segments -> as long as it takes to transmit a respective packet
    this->criticalGuardBandSegmentDuration = calculateGuardBandDurationFromPayloadSize(this->maximumCriticalPayloadSize);
    this->noncriticalGuardBandSegmentDuration = calculateGuardBandDurationFromPayloadSize(this->maximumNoncriticalPayloadSize);

    // calculate sending segments
    this->criticalSendingSegmentDuration = this->criticalIntervalDuration - this->criticalGuardBandSegmentDuration;
    this->noncriticalSendingSegmentDuration = this->noncriticalIntervalDuration - this->noncriticalGuardBandSegmentDuration;

    //get number supported Priorities
    this->numberSupportedPriorities = this->par("supportedPriorities");
    this->switchMAC = getParentModule()->par("mac");
    this->portIndex = getParentModule()->getIndex();

    // get critical class priorities
    this->criticalPriorityClasses = getCriticalClasses();
    this->isCritical = setUpVectorCriticalPriorities(this->criticalPriorityClasses);

    // get the time offset
    this->timeOffset = getTimeOffset();
    if (this->timeOffset > this->periodDuration)
    {
        this->timeOffset = calculateTimeModulo(this->timeOffset, this->periodDuration);
    }

    // get the initial state (= currentState) defined by offset
    this->currentState = getInitialStateAndCalculateTriggerTimer(this->timeOffset);
    this->nextState = getNextState(currentState);

    this->arbiterIsBusy = false;
    this->frameStoredInBuffer = false;

    initializeTimerMessage();
    printInitializationConfiguration();
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::getPeriodDuration()
{
    double periodDoubleValue = this->par("periodDuration").doubleValue();
    simtime_t period = convertDoubleIntoTime(periodDoubleValue);
    return period;
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::getCriticalIntervalDuration()
{
    double criticalIntervalDoubleValue = this->par("criticalIntervalDuration").doubleValue();
    simtime_t criticalInterval = convertDoubleIntoTime(criticalIntervalDoubleValue);
    return criticalInterval;
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::convertDoubleIntoTime(double valueToBeConverted)
{
    simtime_t convertedTimeValue = SimTime(valueToBeConverted, SIMTIME_NS);
    return convertedTimeValue;
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::calculateGuardBandDurationFromPayloadSize(int payload)
{
    long portSpeed = getPortSpeed();
    int packetSize = setEthPacketSizeFromPayload(payload);
    simtime_t guardBandSegment = ((packetSize + 1) * 8) / (portSpeed * 1e6);
    return guardBandSegment;
}

/*
 *
 */
long ArbiterIEEE802_1Qbv::getPortSpeed()
{
    // initialize port speed
    long portSpeed = 0;

    // get the output port of shaper
    cGate *egressPort = this->gate("out");

    while (egressPort->getChannel() == nullptr)
    {
        egressPort = egressPort->getNextGate();
    }

    // get the corresponding channel to output port
    cChannel *outputChannel = egressPort->getChannel();

    // check if channel equal nullptr -> port is not connected throw error
    if(outputChannel == nullptr)
    {
        //LOG ERROR
        EV << "Error in [" << this->getClassName() << "]: The channel data rate could not be found.\n";
        throw cRuntimeError("ERROR: The channel data rate could not be found in switch.");
    }

    // get data rate from, convert it into double and calculate port speed
    portSpeed = (long) outputChannel->par("datarate").doubleValue() / 1e6;

    // check if a port speed was calculated -> throw error if not
    if(portSpeed == 0)
    {
        EV << "Error in ["<< this->getClassName() << "]: Computation of port speed failed.\n";
        throw cRuntimeError("ERROR: Computation of port speed failed.");
    }

    return portSpeed;
}

/*
 *
 */
int ArbiterIEEE802_1Qbv::setEthPacketSizeFromPayload(int payload)
{
    if(payload > _MAX_ETH_PAYLOAD_)
    {
        EV << "ERROR in [" << this->getClassName() << "]: set payload (= " << payload << ") is greater than maximum "
                "payload (= "<< _MAX_ETH_PAYLOAD_ <<").";
        throw cRuntimeError("ERROR: set payload is greater than maximum payload");
    }

    int ethernetPacketSize = payload + _FRAME_OVERHEAD_ + _PACKET_OVERHEAD_ + _INTERPACKET_GAP_;
    return ethernetPacketSize;
}

/*
 *
 */
std::vector<int> ArbiterIEEE802_1Qbv::getCriticalClasses()
{
    const char* criticalPrioritiesString = this->par("criticalPriorityClasses");
    if(std::strcmp(criticalPrioritiesString, "") == 0)
    {
        EV << "["<< this->getClassName() <<"] No critical priority classes are set!" << "\n";
    }
    std::vector<int> criticalPrioritesInt = cStringTokenizer(criticalPrioritiesString).asIntVector();

    return criticalPrioritesInt;
}

std::vector<bool> ArbiterIEEE802_1Qbv::setUpVectorCriticalPriorities(std::vector<int> criticalPriorities)
{
    std::vector<bool> criticalPriorityVector;
    criticalPriorityVector.resize(this->numberSupportedPriorities, false);

    for(int iterator = 0; iterator< criticalPriorities.size(); iterator++)
    {
        int priority = criticalPriorities[iterator];
        criticalPriorityVector[priority] = true;
    }

    return criticalPriorityVector;
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::getTimeOffset()
{
    double doubleOffset = this->par("timeOffset").doubleValue();
    simtime_t offset = convertDoubleIntoTime(doubleOffset);
    return offset;
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Qbv::calculateTimeModulo(simtime_t numeratorTime, simtime_t denominatorTime)
{
    simtime_t remainder = numeratorTime;
    while(remainder >= denominatorTime)
    {
        remainder = remainder - denominatorTime;
    }

    return remainder;
}

/*
 *
 */
int ArbiterIEEE802_1Qbv::getInitialStateAndCalculateTriggerTimer(simtime_t offset)
{
    int initialState = 0;
    simtime_t timer = 0.0;

    if(this->timeOffset < this->criticalIntervalDuration)
    {
        if(this->timeOffset < this->criticalSendingSegmentDuration)
        {
            initialState = CRITICAL_SENDING;
            timer = (this->criticalSendingSegmentDuration  - this->timeOffset);
        }
        else
        {
            initialState = CRITICAL_GUARDBAND;
            timer = (this->criticalSendingSegmentDuration + this->criticalGuardBandSegmentDuration - this->timeOffset);
        }
    }
    else
    {
        if(this->timeOffset < (this->periodDuration - this->noncriticalGuardBandSegmentDuration))
        {
            initialState = NONCRITICAL_SENDING;
            timer = (this->periodDuration - this->noncriticalGuardBandSegmentDuration - this->timeOffset);
        }
        else
        {
            initialState = NONCRITICAL_GUARDBAND;
            timer = this->periodDuration - this->timeOffset;
        }
    }

    this->triggerTimer = timer;
    return initialState;
}

/*
 *
 */
int ArbiterIEEE802_1Qbv::getNextState(int currentState)
{
    switch(currentState)
    {
        case CRITICAL_SENDING:
            this->nextState = CRITICAL_GUARDBAND;
            StatisticManager::getInstance().getTimeAwareShaperStatistics()->criticalSendingSegmentStarts(this->switchMAC, this->portIndex);
            EV << "critical sending state";
            break;
        case CRITICAL_GUARDBAND:
            this->nextState = NONCRITICAL_SENDING;
            StatisticManager::getInstance().getTimeAwareShaperStatistics()->criticalGuardbandSegmentStarts(this->switchMAC, this->portIndex);
            EV << "critical guard band state";
            break;
        case NONCRITICAL_SENDING:
            this->nextState = NONCRITICAL_GUARDBAND;
            StatisticManager::getInstance().getTimeAwareShaperStatistics()->noncriticalSendingSegmentStarts(this->switchMAC, this->portIndex);
            EV << "noncritical sending state";
            break;
        case NONCRITICAL_GUARDBAND:
            this->nextState = CRITICAL_SENDING;
            StatisticManager::getInstance().getTimeAwareShaperStatistics()->noncriticalGuardbandSegmentStarts(this->switchMAC, this->portIndex);
            EV << "noncritical guard band state";
            break;
        default:
            EV << "ERROR: invalid state. State does not exist";
    }

    return this->nextState;
}

/*
 *
 */
void ArbiterIEEE802_1Qbv::initializeTimerMessage()
{
    this->segmentTriggerTimerMessage = new cMessage();
    this->segmentTriggerTimerMessage->setName("TimeAwareTrigger");
    this->segmentTriggerTimerMessage->addPar("nextState").setLongValue(this->nextState);
    scheduleAt(this->triggerTimer, this->segmentTriggerTimerMessage);
}

/*
 *
 */
void ArbiterIEEE802_1Qbv::printInitializationConfiguration()
{
    // print initialization setups
    EV << "[" << this->getClassName() << "] is activated traffic shaper \n";
    EV << "[" << this->getClassName() << "] period duration: \t" << this->periodDuration << "\n";
    EV << "[" << this->getClassName() << "] critical interval duration: \t" << this->criticalIntervalDuration << "\n";
    EV << "[" << this->getClassName() << "] noncritical interval duration: \t" << this ->noncriticalIntervalDuration << "\n";
    EV << "[" << this->getClassName() << "] critical guard band duration: \t" << this->criticalGuardBandSegmentDuration << "\n";
    EV << "[" << this->getClassName() << "] noncritical guard band duration: \t"<< this->noncriticalGuardBandSegmentDuration << "\n";
    EV << "[" << this->getClassName() << "] critical payload size: \t"<< this->maximumCriticalPayloadSize << "\n";
    EV << "[" << this->getClassName() << "] noncritical payload size: \t"<< this->maximumNoncriticalPayloadSize << "\n";
    EV << "[" << this->getClassName() << "] initial state: \t" << this->currentState << "\n";
    EV << "[" << this->getClassName() << "] next state: \t" << this->nextState << "\n";
}

/* ------- HANDLE MESSAGES ------ */
void ArbiterIEEE802_1Qbv::handleMessage(cMessage* message)
{
    if(message->isSelfMessage())
    {
        // handle timer message
        dealWithInternalMessage(message);
    }
    else
    {
        // handle external message
        dealWithExternalMessage(message);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~ INTERNAL MESSAGES ~~~~~~~~~~~~~~~~~~~~ */

void ArbiterIEEE802_1Qbv::dealWithInternalMessage(cMessage* message)
{
    if(message == this->segmentTriggerTimerMessage)
    {
        dealWithSegmentTimerMessage();

        if(this->arbiterIsBusy == false && this->frameStoredInBuffer)
        {
            controlArbitrationBufferStatusList(this->currentBufferStatusList);
        }
    }
    else if (message == this->triggerSelf)
    {
        dealWithTriggerSelf();
    }
}

void ArbiterIEEE802_1Qbv::dealWithSegmentTimerMessage()
{
    // update the current state
    this->currentState = (int) this->segmentTriggerTimerMessage->par("nextState");
    // update the next state
    this->nextState = getNextState(this->currentState);
    this->segmentTriggerTimerMessage->par("nextState").setLongValue(this->nextState);
    // get the duration of the current state segment
    simtime_t segmentDuration = getSegmentDurationFromCurrentState(this->currentState);
    this->triggerTimer = simTime() + segmentDuration;
    scheduleAt(this->triggerTimer, this->segmentTriggerTimerMessage);
}

simtime_t ArbiterIEEE802_1Qbv::getSegmentDurationFromCurrentState(int state)
{
    simtime_t segmentDuration;
    switch(state)
    {
        case CRITICAL_SENDING:
            segmentDuration = this->criticalSendingSegmentDuration;
            break;
        case CRITICAL_GUARDBAND:
            this->nextState = NONCRITICAL_SENDING;
            segmentDuration = this->criticalGuardBandSegmentDuration;
            break;
        case NONCRITICAL_SENDING:
            segmentDuration = this->noncriticalSendingSegmentDuration;
            break;
        case NONCRITICAL_GUARDBAND:
            segmentDuration = this->noncriticalGuardBandSegmentDuration;
            break;
        default:
            EV << "ERROR: invalid state. State does not exist";
    }

    return segmentDuration;
}

void ArbiterIEEE802_1Qbv:: dealWithTriggerSelf()
{
    sendFrameOut();
    sendBufferRequest();
    this->arbiterIsBusy = false;
}

void ArbiterIEEE802_1Qbv::sendFrameOut()
{
    if (this->ethernetFrameWaitForTransmission == nullptr)
    {
        EV << "["<< this->getClassName() << "] ERROR: No frame to send!" << "\n";
        throw cRuntimeError("ERROR: No frame to send!");
    }

    int streamID = ethernetFrameWaitForTransmission->getStreamID();
    EV<<"finished sending � " << streamID <<"\n";
    StatisticManager::getInstance().getLinkTransmissionStatistics()->ethernetFrameFinishedSending(switchMAC, portIndex, streamID);
    send(this->ethernetFrameWaitForTransmission,"out");
    EV << "["<< this->getClassName() << "] send next frame out"<< "\n";
}

void ArbiterIEEE802_1Qbv::sendBufferRequest()
{
    BufferControlMessage* authorizedBufferMessage = new BufferControlMessage();
    authorizedBufferMessage->setMessageInformation(BUFFER_LEVEL_REQUEST);
    send(authorizedBufferMessage,"control$o");
}

/* ~~~~~~~~~~~~~~~~~~ EXTERNAL MESSAGE~~~~~~~~~~~~~~~~~~~~~~ */
void ArbiterIEEE802_1Qbv::dealWithExternalMessage(cMessage* message)
{
    cGate* arrivalGate = message->getArrivalGate();
        const char* gateName = arrivalGate->getName();
        bool controlMessage = strcmp(gateName, "control$i");
        bool ethernetFrame = strcmp(gateName, "in");

        if(!controlMessage)
        {
            dealWithControlMessage(message);
        }
        else if (!ethernetFrame)
        {
            dealWithEthernetFrame(message);
        }
}

void ArbiterIEEE802_1Qbv::dealWithControlMessage(cMessage* controlMessage)
{
    this->frameStoredInBuffer = true;
    BufferControlMessage* bufferControlMessage = check_and_cast<BufferControlMessage*> (controlMessage);
    if (bufferControlMessage == nullptr)
    {
        throw cRuntimeError("ERROR: Could not cast to BufferControlMessage in ArbiterIEEE802_1Qbv!");
    }

    int controlMessageInformation = bufferControlMessage->getMessageInformation();

    //check message type
    switch(controlMessageInformation)
    {
        case BUFFER_LEVEL_RESPONSE:
            EV << "[" << this->getClassName() <<"] BUFFER_LEVEL_RESPONSE \n";
            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();

            controlArbitrationBufferStatusList(this->currentBufferStatusList);
            break;

        case BUFFER_LEVEL_UPDATE:
            EV << "[" << this->getClassName() << "] BUFFER_LEVEL_UPDATE \n";
            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();

            if(this->arbiterIsBusy == false)
            {
                controlArbitrationBufferStatusList(this->currentBufferStatusList);
            }
            break;

        default:
            //Sanity check
           throw cRuntimeError("Unrecognized message type!");
           break;
    }
    delete bufferControlMessage;
}

void ArbiterIEEE802_1Qbv::controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList)
{
    std::vector<ethernetHeaderInformation*> updatedBufferStatusList;
    int authorizedBuffer = -1;

    updatedBufferStatusList = prepareBufferStatusList(bufferStatusList);
    authorizedBuffer = arbitrateBufferStatusList(updatedBufferStatusList);

    if (authorizedBuffer != -1 )
    {
        sendBufferAuthorizedBuffer(authorizedBuffer);
    }
    else
    {
        EV << "["<<getClassName()<<"] no frames ready or buffers empty \n";
    }
}

std::vector<ethernetHeaderInformation*> ArbiterIEEE802_1Qbv::prepareBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList)
{
    for (int priorityIterator = 0 ; priorityIterator < this->numberSupportedPriorities; priorityIterator++)
    {
        if(bufferStatusList[priorityIterator]->payloadLength > 0)
        {
            bufferStatusList[priorityIterator]->ready = true;
        }
        else
        {
            bufferStatusList[priorityIterator]->ready = false;
        }
    }
    return bufferStatusList;
}

int ArbiterIEEE802_1Qbv::arbitrateBufferStatusList(std::vector<ethernetHeaderInformation*> bufferStatusList)
{
    if(this->currentState == CRITICAL_SENDING)
    {
        for (int priority = this->numberSupportedPriorities-1 ; priority >= 0; priority--)
        {
            if(bufferStatusList[priority]->ready && isCritical[priority] == true)
            {
                //Take element
                EV << "["<<this->getClassName()<<"] Priority: "<< priority << "\n";
                return priority;
            }
        }
    }
    else if(this->currentState == NONCRITICAL_SENDING)
    {
        for (int priority = this->numberSupportedPriorities-1 ; priority >= 0; priority--)
        {
            if(bufferStatusList[priority]->ready && isCritical[priority] == false)
            {
                //Take element
                EV << "["<<this->getClassName()<<"] Priority: "<< priority << "\n";
                return priority;
            }
        }
    }
    return -1;
}

void ArbiterIEEE802_1Qbv::sendBufferAuthorizedBuffer(int authorizedBuffer)
{
    BufferControlMessage* authorizedBufferMessage = new BufferControlMessage();

    authorizedBufferMessage->setMessageInformation(SEND_AUTHORISATION);
    authorizedBufferMessage->setAuthorizedBuffer(authorizedBuffer);

    this->arbiterIsBusy = true;
    send(authorizedBufferMessage,"control$o");
}

/* ~~~~~~~~~ EXTERNAL MESSAGE - ETHERNET FRAME ~~~~~~~~~~~~~~~~~ */
void ArbiterIEEE802_1Qbv::dealWithEthernetFrame(cMessage* frame)
{
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*> (frame);
    int streamID = ethernetFrame->getStreamID();
    if (ethernetFrame == nullptr)
    {
        EV << "["<< this->getClassName() <<"] ERROR: Could not cast message to EthernetFrame!" << "\n";
        throw cRuntimeError("ERROR: Could not cast message to EthernetFrame!");
    }
    this->ethernetFrameWaitForTransmission = ethernetFrame;
    EV<<"start sending � " << streamID <<"\n";
    StatisticManager::getInstance().getLinkTransmissionStatistics()->ethernetFrameStartedSending(switchMAC, portIndex, streamID);
    simtime_t transmissionPathDelay;
    transmissionPathDelay = calculateTransmissionDelay(ethernetFrame);

    setTriggerDelayEnd(transmissionPathDelay);
}

simtime_t ArbiterIEEE802_1Qbv::calculateTransmissionDelay(EthernetFrame* ethernetFrame)
{
    int payload = ethernetFrame->getPayloadSize();
    long portSpeed = getPortSpeed();
    int ethernetPacketSize = setEthPacketSizeFromPayload(payload);
    simtime_t transmissionPathDelay;

    transmissionPathDelay = (ethernetPacketSize * 8) / (portSpeed * 1e6);

    if (transmissionPathDelay.dbl() < 0)
    {
        EV << "["<< this->getClassName() << "] ERROR: Incorrect Frame Path Delay!" << "\n";
        throw cRuntimeError("ERROR: Incorrect Frame Path Delay!");
    }

    return transmissionPathDelay;
}

void ArbiterIEEE802_1Qbv::setTriggerDelayEnd(simtime_t transmissionPathDelay)
{
    simtime_t triggerTime;

    //prepare time Trigger
    this->triggerSelf->setName("triggerDelayEnd");
    triggerTime = simTime() + transmissionPathDelay;

    scheduleAt(triggerTime, this->triggerSelf);
    this->arbiterIsBusy = true;
}
