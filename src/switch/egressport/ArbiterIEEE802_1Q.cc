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

#include "ArbiterIEEE802_1Q.h"

ArbiterIEEE802_1Q::ArbiterIEEE802_1Q()
{

}

ArbiterIEEE802_1Q::~ArbiterIEEE802_1Q()
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
void ArbiterIEEE802_1Q::initialize()
{
    this->parentModul = getParentModule();
    this->numberPriorities = (int) this->par("supportedPriorities");
    this->arbiterIsBusy = false;
    this->ethernetFrameIsWaiting = false;

    calculateTransmissionSpeed();
}

/*
 *
 */
void ArbiterIEEE802_1Q::handleMessage(cMessage* message)
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
void ArbiterIEEE802_1Q::dealWithInternalMessage(cMessage* message)
{
    if (message == this->triggerSelf)
    {
        if (strcmp("triggerDelayEnd", message->getName()) == 0)
        {
            sendFrameOut();
            sendBufferRequest();
            this->arbiterIsBusy = false;
        }
    }
}

/*
 *
 */
void ArbiterIEEE802_1Q::dealWithExternalMessage(cMessage* message)
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
void ArbiterIEEE802_1Q::dealWithEthernetFrame(cMessage* frame)
{
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*> (frame);
    if (ethernetFrame == nullptr)
    {
        EV << "[ArbiterIEEE802_1Q] ERROR: Could not cast to EthernetFrame in ArbiterIEEE802_1Q!" << "\n";
        throw cRuntimeError("[ArbiterIEEE802_1Q] ERROR: Could not cast to EthernetFrame in ArbiterIEEE802_1Q!");
    }
    this->ethernetFrameIsWaiting = true;
    this->ethernetFrameWaitForSend = ethernetFrame;

    simtime_t transmissionPathDelay;

    transmissionPathDelay = calculateTransmissionDelay(ethernetFrame);

    settriggerDelayEnd(transmissionPathDelay);
}

/*
 *  Control Communication to BuffersModule
 */
void ArbiterIEEE802_1Q::dealWithControlMessage(cMessage* controlMessage)
{
    BufferControlMessage* bufferControlMessage = check_and_cast<BufferControlMessage*> (controlMessage);
    if (bufferControlMessage == nullptr)
    {
        EV << "[ArbiterIEEE802_1Q] ERROR: Could not cast to BufferControlMessage in ArbiterIEEE802_1Q!" << "\n";
        throw cRuntimeError("[ArbiterIEEE802_1Q] ERROR: Could not cast to BufferControlMessage in ArbiterIEEE802_1Q!");
    }

    int controlMessageInformation = bufferControlMessage->getMessageInformation();

    //check message type
    switch(controlMessageInformation)
    {
        case BUFFER_LEVEL_RESPONSE:
            EV << "[ArbiterIEEE802_1Q] BUFFER_LEVEL_RESPONSE \n";
            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();
            controlArbitrationBufferStatusList(currentBufferStatusList);
            delete controlMessage;
            break;

        case BUFFER_LEVEL_UPDATE:
            EV << "[ArbiterIEEE802_1Q] BUFFER_LEVEL_UPDATE \n";
            this->currentBufferStatusList = bufferControlMessage->getInformationPerBuffer();

            if(!this->arbiterIsBusy)
            {
                controlArbitrationBufferStatusList(currentBufferStatusList);
            }

            delete controlMessage;
            break;

        default:
            //Should not happen
            EV << "[ArbiterIEEE802_1Q] ERROR: Unrecognised message type!" << "\n";
            throw cRuntimeError("[ArbiterIEEE802_1Q] ERROR: Unrecognised message type!");

            break;
    }
}

/*
 *
 */
void ArbiterIEEE802_1Q::controlArbitrationBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    std::vector<ethernetHeaderInformation*> updatedBufferStatusList;
    int arbiterResult;

    updatedBufferStatusList = prepareBufferStatusList(BufferStatusList);

    arbiterResult = arbitrateBufferStatusList(updatedBufferStatusList);

    if (arbiterResult >= 0)
    {
        sendBufferAuthorizedBuffer(arbiterResult);
    }
}

/*
 *
 */
std::vector<ethernetHeaderInformation*> ArbiterIEEE802_1Q::prepareBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    for (int i = 0 ; i < this->numberPriorities; i++)
    {
        if(BufferStatusList[i]->payloadLength > 0)
        {
            BufferStatusList[i]->ready = true;
        }
        else
        {
            BufferStatusList[i]->ready = false;
        }
    }
    return BufferStatusList;
}

/*
 *  Choose one of the buffers where length != 0
 */
int ArbiterIEEE802_1Q::arbitrateBufferStatusList(std::vector<ethernetHeaderInformation*> BufferStatusList)
{
    for (int i = this->numberPriorities -1 ; i >= 0; i--)
    {
        if(BufferStatusList[i]->ready)
        {
            //Take element
            EV << "[ArbiterIEEE802_1Q] take element : "<< i << "\n";
            return(i);
        }
    }
    std::string moduleName =  this->parentModul->par("arbiterType");
    EV << "[" << moduleName << "] no frames left \n";
    return -1;
}


/*
 *  send out authorized buffer message
 */
void ArbiterIEEE802_1Q::sendBufferAuthorizedBuffer(int authorizedBuffer)
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
void ArbiterIEEE802_1Q::sendBufferRequest()
{
    BufferControlMessage* authorizedBufferMessage = new BufferControlMessage();

    authorizedBufferMessage->setMessageInformation(BUFFER_LEVEL_REQUEST);

    send(authorizedBufferMessage,"control$o");
}

/*
 *
 */
simtime_t ArbiterIEEE802_1Q::calculateTransmissionDelay(EthernetFrame* ethernetFrame)
{
    int ethernetFrameSize = ethernetFrame->getFrameSize();
    int ehternetPhyFrameSize = ethernetFrameSize + _PACKET_OVERHEAD_ + _INTERPACKET_GAP_;
    simtime_t transmissionPathDelay;

    transmissionPathDelay = (ehternetPhyFrameSize * 8) / (this->channelTransmissionRate * 1e6);

    if (transmissionPathDelay.dbl() < 0)
    {
        EV << "[ArbiterIEEE802_1Q] ERROR: Incorrect Frame Path Delay!" << "\n";
        throw cRuntimeError("[ArbiterIEEE802_1Q] ERROR: Incorrect Frame Path Delay!");
    }
    //EV << "[herelokiloki]" << transmissionPathDelay << "\n";
    return transmissionPathDelay;
}

/*
 *
 */
void ArbiterIEEE802_1Q::calculateTransmissionSpeed()
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
void ArbiterIEEE802_1Q::settriggerDelayEnd(simtime_t transmissionPathDelay)
{
    simtime_t triggerTime;

    //prepare time Trigger
    this->triggerSelf->setName("triggerDelayEnd");
    triggerTime = simTime() + transmissionPathDelay;

    scheduleAt(triggerTime, this->triggerSelf);
    this->arbiterIsBusy = true;
}

/*
 *
 */
void ArbiterIEEE802_1Q::sendFrameOut()
{
    if (ethernetFrameWaitForSend == nullptr)
    {
        EV << "[ArbiterIEEE802_1Q] ERROR: No frame to send!" << "\n";
        throw cRuntimeError("[ArbiterIEEE802_1Q] ERROR: No frame to send!");
    }

    send(this->ethernetFrameWaitForSend,"out");
    this->ethernetFrameIsWaiting = false;

    EV << "[ArbiterIEEE802_1Q] send next frame out"<< "\n";
}
