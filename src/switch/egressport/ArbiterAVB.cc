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

#include "ArbiterAVB.h"

ArbiterAVB::ArbiterAVB()
{
    //this->ethernetFrameWaitForSend = new EthernetFrame;
}

ArbiterAVB::~ArbiterAVB()
{
    cancelAndDelete(this->triggerSelf);
    if (this->ethernetFrameIsWaiting == true)
    {
        delete(this->ethernetFrameWaitForSend);
    }
}

/*
 *
 */
void ArbiterAVB::initialize()
{
    this->parentModul = getParentModule();
    this->numberPriorities = (int) this->par("supportedPriorities");
    this->macAddress = this->parentModul->getParentModule()->par("mac");
    this->egressPortIndex = this->parentModul->getIndex();
    this->arbiterIsBusy = false;
    this->ethernetFrameIsWaiting = false;

    calculateTransmissionSpeed();
    initializeAVBClassInformation();



    //debugStatusInitialization();

    //statistics vector for collecting the credit utilization for a resolution given by the processing of frames
    char name[32];
    sprintf(name, "credit-value-stream-A");
    this->creditValueStatisticStreamA.setName(name);
    sprintf(name, "credit-value-stream-B");
    this->creditValueStatisticStreamB.setName(name);
}

/*
 *
 */
void ArbiterAVB::calculateTransmissionSpeed()
{
    this->parentGateOut = this->parentModul->gate("out");
    int safeGuard = 10;
    while (parentGateOut->getChannel() == nullptr && safeGuard > 0)
    {
        parentGateOut = parentGateOut->getNextGate();
        safeGuard--;
    }
    this->parentChannelOut = this->parentGateOut->getChannel();
    this->channelTransmissionRate = (long)this->parentChannelOut->par("datarate").doubleValue() / 1e6;
}

/*
 *
 */
void ArbiterAVB::initializeAVBClassInformation()
{
    this->classInformations.resize(this->numberPriorities);
    for (int i = 0; i< this->numberPriorities;i++)
    {
        this->classInformations[i] = new arbiterAVBClass();
        //this->classInformations[i]->frameBufferID = i;

        this->classInformations[i]->classCreditValue = 0;
        this->classInformations[i]->classFrameWaiting = false;
        this->classInformations[i]->classStatus = CLASS_NOTSHAPED;

        this->classInformations[i]->classIsShaped = 0;
        this->classInformations[i]->sendSlope = 1;
        this->classInformations[i]->idleSlope = 1;
        this->classInformations[i]->lastArbitrationTimeStamp = simTime();
        this->classInformations[i]->timerIsScheduled = false;
    }
    std::string configFile = this->par("configFileName");

    this->avbTableVector = ConfigurationReader::getInstance()->getAVBTableEntriesFromInputFile(configFile);
    loadAVBClassInformationFromFile();
}

/*
 *    iterator= [0:macAdress, 1:egressPortIndex, 2:Priority, 3:sendSlope, 4:idleSlope]
 */
void ArbiterAVB::loadAVBClassInformationFromFile()
{
    for (int iterator = 0; iterator < avbTableVector.size(); iterator++)
    {
        if (this->avbTableVector[iterator].size() != 5)
        {
            EV_ERROR << "[AVBArbiter] error with entries \n";
            throw cRuntimeError("[AVBArbiter] Config file contains bad entries!");
        }
        //line= [0:macAdress, 1:egressPortIndex, 2:Priority, 3:sendSlope, 4:idleSlope]
        if((this->avbTableVector[iterator][0]) == this->macAddress)
        {
            //for my port or wildcard
            if((this->avbTableVector[iterator][1]) == this->egressPortIndex || (this->avbTableVector[iterator][1]) == -1)
            {
                int entryPriority = (this->avbTableVector[iterator][2]);

                this->classInformations[entryPriority]->sendSlope = (this->avbTableVector[iterator][3]);
                this->classInformations[entryPriority]->idleSlope = (this->avbTableVector[iterator][4]);
                this->classInformations[entryPriority]->classStatus = CLASS_IDLE;
                this->classInformations[entryPriority]->classIsShaped = 1;

                EV << "[AVBArbiter] prepared struct for Priority: " << entryPriority << " with sendSlope " << this->classInformations[entryPriority]->sendSlope <<  "\n";
                EV << "[AVBArbiter] prepared struct for Priority: " << entryPriority << " with idleSlope " << this->classInformations[entryPriority]->idleSlope <<  "\n";
            }
        }
    }
}

/*
 *
 */
void ArbiterAVB::handleMessage(cMessage* message)
{
    if(message->isSelfMessage())
    {
        dealWithInternalMessage(message);
    }
    else
    {
        dealWithExternalMessage(message);
    }
}

/*
 *
 */
void ArbiterAVB::dealWithInternalMessage(cMessage* message)
{
    if (message == this->triggerSelf)
    {
        if (strcmp("triggerDelayEnd", message->getName()) == 0)
        {
            //EV << "[ArbiterAVB] Revieve Trigger triggerDelayEnd \n";
            sendFrameOut();
            sendBufferRequest();
            this->arbiterIsBusy = false;
        }
        if (strcmp("triggerCreditRecharged", message->getName()) == 0)
        {
            int priority;
            priority = message->par("priority");

            sendBufferRequest();

            this->classInformations[priority]->timerIsScheduled = false;
        }
    }
}

/*
 *
 */
void ArbiterAVB::dealWithExternalMessage(cMessage* message)
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

/*
 *
 */
void ArbiterAVB::dealWithEthernetFrame(cMessage* message)
{
    EthernetFrame* ethernetFrame = checkAndCastEthernetFrame(message);

    this->ethernetFrameIsWaiting = true;
    this->ethernetFrameWaitForSend = ethernetFrame;

    simtime_t transmissionPathDelay;

    transmissionPathDelay = calculateTransmissionDelay(ethernetFrame);

    settriggerDelayEnd(transmissionPathDelay);
}

/*
 *
 */
EthernetFrame* ArbiterAVB::checkAndCastEthernetFrame(cMessage* message)
{
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*>(message);
    if (ethernetFrame == nullptr)
    {
        EV << "[ArbiterAVB] ERROR: Could not cast to EthernetFrame in ArbiterAVB!" << "\n";
        throw cRuntimeError("[ArbiterAVB] ERROR: Could not cast to EthernetFrame in ArbiterAVB!");
    }
    return ethernetFrame;
}

/*
 *
 */
BufferControlMessage* ArbiterAVB::checkAndCastBufferControlMessage(cMessage* message)
{
    BufferControlMessage* bufferControlMessage = check_and_cast<BufferControlMessage*> (message);
    if (bufferControlMessage == nullptr)
    {
        EV << "[ArbiterAVB] ERROR: Could not cast to BufferControlMessage in ArbiterAVB!" << "\n";
        throw cRuntimeError("[ArbiterAVB] ERROR: Could not cast to BufferControlMessage in ArbiterAVB!");
    }
    return bufferControlMessage;
}

/*
 *  Control Communication to BuffersModule
 */
void ArbiterAVB::dealWithControlMessage(cMessage* message)
{
    BufferControlMessage* bufferControlMessage = checkAndCastBufferControlMessage(message);

    int controlMessageInformation = bufferControlMessage->getMessageInformation();

    //check message type
    switch(controlMessageInformation)
    {
        case BUFFER_LEVEL_RESPONSE:
            //EV << "[ArbiterAVB] BUFFER_LEVEL_RESPONSE \n";
            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();

            //arbitration start
            controlArbitrationBufferStatusList(currentBufferStatusList);

            delete bufferControlMessage;
            break;

        case BUFFER_LEVEL_UPDATE:

            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();

            if (! this->arbiterIsBusy)
            {
                //EV << "[ArbiterAVB] BUFFER_LEVEL_UPDATE \n";
                //arbitration start
                controlArbitrationBufferStatusList(currentBufferStatusList);
            }
            else
            {
                EV << "[ArbiterAVB] BUFFER_LEVEL_UPDATE but busy \n";
            }

            delete bufferControlMessage;
            break;

        default:
            //Should not happen
            EV << "[ArbiterAVB] ERROR: Unrecognised message type!" << "\n";
            throw cRuntimeError("[ArbiterAVB] ERROR: Unrecognised message type!");
            break;
    }
}

/*
 *
 */
void ArbiterAVB::controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    std::vector<ethernetHeaderInformation*> updatedBufferStatusList;
    int authorizedBuffer = -1;

    //calculate and prepare credits
    prepareClassInformations(BufferStatusList);
    // update Status List
    updatedBufferStatusList = prepareBufferStatusList(BufferStatusList);
    // take one of them
    authorizedBuffer = arbitrateBufferStatusList(updatedBufferStatusList);

    //collect statistics
    collectCreditsStatistics(BufferStatusList);

    if (authorizedBuffer != -1 )
    {
        sendBufferAuthorizedBuffer(authorizedBuffer);
    }
    else
    {
        setRechargeTime();
        EV << "[ArbiterAVB] no frames ready or buffers empty \n";
    }
    updateClassInformationsStatus(authorizedBuffer);
    debugStatus();
}

/*
 *
 */
void ArbiterAVB::prepareClassInformations(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    for(int priority = 0 ; priority < this->numberPriorities; priority++)
    {
        if (this->classInformations[priority]->classStatus != CLASS_NOTSHAPED)
        {
           //if frame is new
           if (this->classInformations[priority]->classFrameWaiting == false)
           {
               if(BufferStatusList[priority]->payloadLength > 0)
               {
                   this->classInformations[priority]->classFrameWaiting = true;
                   this->classInformations[priority]->lastArbitrationTimeStamp = simTime();
                   this->classInformations[priority]->classStatus = CLASS_WAITING;
               }
           }
           //if no frame left
           if (this->classInformations[priority]->classFrameWaiting == true)
           {
               if(BufferStatusList[priority]->payloadLength == 0)
               {
                   this->classInformations[priority]->classFrameWaiting = false;
                   this->classInformations[priority]->classCreditValue = 0;
                   this->classInformations[priority]->classStatus = CLASS_IDLE;
               }
           }
           if (this->classInformations[priority]->classFrameWaiting == true)
           {
               if(BufferStatusList[priority]->payloadLength > 0)
               {
                   //nothing changed
               }
           }
        }
    }
    calculateClassInformationCredits();
}

/*
 *
 */
void ArbiterAVB::calculateClassInformationCredits()
{
    for(int priority = 0 ; priority < this->numberPriorities; priority++)
    {
        switch(this->classInformations[priority]->classStatus)
        {
            //empty nothing do
            case CLASS_IDLE:
                //EV << "[ArbiterAVB] Class" << i << " is idle \n";
                break;

            //is new nothing to calculate
            case CLASS_WAITING:
                //EV << "[ArbiterAVB] Class" << i << " is waiting \n";
                break;

            // was blocked --> increase credits
            case CLASS_BLOCKED:
                //EV << "[ArbiterAVB] Class" << i << " is blocked \n";
                calculateCredits(priority);
                break;

            // was sending --> decrease credits
            case CLASS_SENDING:
                //EV << "[ArbiterAVB] Class" << i << " is sending \n";
                calculateCredits(priority);
                break;

            //nothing to do
            case CLASS_NOTSHAPED:
                //EV << "[ArbiterAVB] Class" << i << " is CLASS_NOTSHAPED \n";
                break;

            default:
                //something went wrong;
                break;
        }
    }
}

/*
 *
 */
void ArbiterAVB::calculateCredits(int priority)
{
    double consumedCredit = 0;
    consumedCredit = calculateCreditValues(priority);

    this->classInformations[priority]->classCreditValue = this->classInformations[priority]->classCreditValue + consumedCredit;
    this->classInformations[priority]->lastArbitrationTimeStamp = simTime();
}

/*
 *
 */
int ArbiterAVB::calculateCreditValues(int priority)
{
    int consumedCredit = 0;
    simtime_t currentTime = simTime();

    double timeDifference = (double) (currentTime.dbl() - this->classInformations[priority]->lastArbitrationTimeStamp.dbl());

    //for sender
    if (this->classInformations[priority]->classStatus == CLASS_SENDING)
    {
        consumedCredit = timeDifference * this->classInformations[priority]->sendSlope * -1 * this->channelTransmissionRate* 1e6;
    }
    //for blocked
    else if (this->classInformations[priority]->classStatus == CLASS_BLOCKED)
    {
        consumedCredit = timeDifference * this->classInformations[priority]->idleSlope * this->channelTransmissionRate* 1e6;
    }
    return consumedCredit;
}

/*
 *
 */
std::vector<ethernetHeaderInformation*> ArbiterAVB::prepareBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    for (int i = 0 ; i < this->numberPriorities; i++)
        {
            if(BufferStatusList[i]->payloadLength > 0 && this->classInformations[i]->classCreditValue >= 0)
            {
                BufferStatusList[i]->ready = true;
            }
            else
            {
                BufferStatusList[i]->ready = false;
            }
        }
    return(BufferStatusList);
}

/*
 *  Choose one of the buffers where length != 0
 */
int ArbiterAVB::arbitrateBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    for (int i = this->numberPriorities -1 ; i >= 0; i--)
    {
        if(BufferStatusList[i]->ready)
        {
            //Take element
            EV << "[ArbiterAVB] Priority: "<< i << "\n";
            return i;
        }
    }
    return -1;
}

/*
 *  send out authorized buffer message
 */
void ArbiterAVB::sendBufferAuthorizedBuffer(int authorizedBuffer)
{
    BufferControlMessage* authorizedBufferMessage = new BufferControlMessage();

    authorizedBufferMessage->setMessageInformation(SEND_AUTHORISATION);
    authorizedBufferMessage->setAuthorizedBuffer(authorizedBuffer);

    this->arbiterIsBusy = true;
    send(authorizedBufferMessage,"control$o");
}

/*
 *
 */
void ArbiterAVB::sendBufferRequest()
{
    BufferControlMessage* authorizedBufferMessage = new BufferControlMessage();

    authorizedBufferMessage->setMessageInformation(BUFFER_LEVEL_REQUEST);

    send(authorizedBufferMessage,"control$o");
}

/*
 *
 */
simtime_t ArbiterAVB::calculateTransmissionDelay(EthernetFrame* ethernetFrame)
{
    int ethernetFrameSize = ethernetFrame->getFrameSize();
    int ehternetPhyFrameSize = ethernetFrameSize + _PACKET_OVERHEAD_ + _INTERPACKET_GAP_;
    simtime_t transmissionPathDelay;

    transmissionPathDelay = (ehternetPhyFrameSize * 8) / (this->channelTransmissionRate * 1e6);

    if (transmissionPathDelay.dbl() < 0)
    {
        EV << "[ArbiterAVB] ERROR: Incorrect Frame Path Delay!" << "\n";
        throw cRuntimeError("[ArbiterAVB] ERROR: Incorrect Frame Path Delay!");
    }

    return transmissionPathDelay;
}

/*
 *
 */
void ArbiterAVB::settriggerDelayEnd(simtime_t transmissionPathDelay)
{
    simtime_t triggerTime;

    //prepare time Trigger
    this->triggerSelf->setName("triggerDelayEnd");
    triggerTime = simTime() + transmissionPathDelay;

    if (this->triggerSelf->isScheduled() == false)
    {
        scheduleAt(triggerTime, this->triggerSelf);
        this->arbiterIsBusy = true;
    }
}

/*
 *
 */
void ArbiterAVB::setRechargeTime()
{
    simtime_t creditRechargeTime;
    for(int i = 0 ; i < this->numberPriorities; i++)
    {
        if (this->classInformations[i]->classStatus != CLASS_NOTSHAPED)
        {
            if (this->classInformations[i]->classCreditValue < 0)
            {
                creditRechargeTime = calculatedRechargeTime(i);
                settriggerCreditRecharged(creditRechargeTime, i);
            }
        }
    }
}

/*
 * prepare time Trigger
 */
void ArbiterAVB::settriggerCreditRecharged(simtime_t calculatedRechargeTime, int priority)
{
    simtime_t triggerTime;

    if (this->classInformations[priority]->timerIsScheduled == false)
    {
        this->triggerSelf->setName("triggerCreditRecharged");
        this->triggerSelf->addPar("priority").setLongValue(priority);


        triggerTime = simTime() + calculatedRechargeTime;

        scheduleAt(triggerTime, this->triggerSelf);

        this->classInformations[priority]->timerIsScheduled = true;
    }
}

/*
 *
 */
simtime_t  ArbiterAVB::calculatedRechargeTime(int priority)
{
    simtime_t creditRechargeTime;

    creditRechargeTime = (-1) * (double) (this->classInformations[priority]->classCreditValue -1) / (double) ((this->classInformations[priority]->idleSlope) * 1e6 * (this->channelTransmissionRate));

    if (creditRechargeTime.dbl() < 0)
    {
        EV << "[ArbiterAVB] ERROR: Incorrect Frame Path Delay!" << "\n";
        throw cRuntimeError("[ArbiterAVB] ERROR: Incorrect Frame Path Delay!");
    }
    return creditRechargeTime;
}

/*
 *
 */
void ArbiterAVB::sendFrameOut()
{
    if (ethernetFrameWaitForSend == nullptr)
    {
        EV << "[ArbiterAVB] ERROR: No frame to send!" << "\n";
        throw cRuntimeError("[ArbiterAVB] ERROR: No frame to send!");
    }
    send(this->ethernetFrameWaitForSend,"out");

    this->ethernetFrameIsWaiting = false;

    EV << "[ArbiterAVB] send next frame out"<< "\n";
}

/*
 *
 */
void ArbiterAVB::updateClassInformationsStatus(int authorizedBuffer)
{
    for(int i = 0 ; i < this->numberPriorities; i++)
    {
        if (this->classInformations[i]->classStatus == CLASS_NOTSHAPED)
        {
            //nada
        }
        else
        {
            if (i == authorizedBuffer)
            {
                this->classInformations[i]->classStatus = CLASS_SENDING;
            }
            else if (this->classInformations[i]->classStatus == CLASS_WAITING)
            {
                this->classInformations[i]->classStatus = CLASS_BLOCKED;
            }

            if (this->classInformations[i]->classCreditValue < 0)
            {
               this->classInformations[i]->classStatus = CLASS_BLOCKED;
            }
        }
    }
}

/*
 *
 */
void ArbiterAVB::collectCreditsStatistics(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    if (this->numberPriorities >= 2)
    {
        if (classInformations[this->numberPriorities-1]->classIsShaped == 1)
        {
            creditValueStatisticStreamA.record(classInformations[this->numberPriorities-1]->classCreditValue);
        }
        if (classInformations[this->numberPriorities-2]->classIsShaped == 1)
        {
            creditValueStatisticStreamB.record(classInformations[this->numberPriorities-2]->classCreditValue);
        }
    }
    else
    {
        EV_ERROR << "[AVBArbiter] Not enough Priorities for AVB \n";
    }
}

/*
 *
 */
void ArbiterAVB::debugStatus()
{
    for(int i = 0 ; i < this->numberPriorities; i++)
    {
        switch(this->classInformations[i]->classStatus)
        {
            //empty nothing do
            case CLASS_IDLE:
                EV << "[ArbiterAVB] Class" << i << " is CLASS_IDLE \n";
                EV << "[ArbiterAVBDebugStatus] "<<  i << " Credits :" << this->classInformations[i]->classCreditValue << "\n";
                break;

            //is new nothing to calculate
            case CLASS_WAITING:
                EV << "[ArbiterAVB] Class" << i << " is CLASS_WAITING \n";
                EV << "[ArbiterAVBDebugStatus] "<<  i << " Credits :" << this->classInformations[i]->classCreditValue << "\n";
                break;

            // was blocked --> increase credits
            case CLASS_BLOCKED:
                EV << "[ArbiterAVB] Class" << i << " is CLASS_BLOCKED \n";
                EV << "[ArbiterAVBDebugStatus] "<<  i << " Credits :" << this->classInformations[i]->classCreditValue << "\n";
                calculateCredits(i);
                break;

            // was sending --> decrease credits
            case CLASS_SENDING:
                EV << "[ArbiterAVB] Class" << i << " is CLASS_SENDING \n";
                EV << "[ArbiterAVBDebugStatus] "<<  i << " Credits :" << this->classInformations[i]->classCreditValue << "\n";
                calculateCredits(i);
                break;

            //nothing to do
            case CLASS_NOTSHAPED:
                //EV << "[ArbiterAVB] Class" << i << " is CLASS_NOTSHAPED \n";
                //EV << "[ArbiterAVBDebugStatus] "<<  i << " Credits :" << this->classInformations[i]->classCreditValue << "\n";
                break;

            default:
                //something went wrong;
                break;
        }
    }
}

void ArbiterAVB::debugStatusInitialization()
{
    for(int i = 0 ; i < this->numberPriorities; i++)
    {
        EV << "[ArbiterAVBDebugStatusInitialization] Class "<<  i << ":\n";
        EV << "[ArbiterAVBDebugStatusInitialization] classCreditValue: " << this->classInformations[i]->classCreditValue << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] classFrameWaiting: " << this->classInformations[i]->classFrameWaiting << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] classIsShaped: " << this->classInformations[i]->classIsShaped << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] classStatus: " << this->classInformations[i]->classStatus << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] idleSlope: " << this->classInformations[i]->idleSlope << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] sendSlope: " << this->classInformations[i]->sendSlope << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] lastArbitrationTimeStamp: " << this->classInformations[i]->lastArbitrationTimeStamp << "\n";
        EV << "[ArbiterAVBDebugStatusInitialization] timerIsScheduled: " << this->classInformations[i]->timerIsScheduled << "\n";
    }

}

