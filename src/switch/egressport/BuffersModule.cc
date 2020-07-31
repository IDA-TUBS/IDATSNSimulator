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

#include "BuffersModule.h"

BuffersModule::BuffersModule()
{
}


BuffersModule::~BuffersModule()
{
    deleteFramesInBuffers();
    deleteBuffers();
}


void BuffersModule::finish()
{

}


void BuffersModule::initialize()
{
    //set the number of supported priorities and maximum buffer size
    setInitinalUserInputs();

    //depending on supported priorities and maximum buffer size set up buffers
    setUpFrameBuffers();
}

void BuffersModule::setInitinalUserInputs()
{
    this->numberSupportedPriorities = getNumberSupportedPriorities();
    this->maximumBufferSize = getMaximumBufferSize();
}


int BuffersModule::getNumberSupportedPriorities()
{
    return this->par("supportedPriorities");
}


int BuffersModule::getMaximumBufferSize()
{
    int maxBytesPerQueue = (int)this->par("maxBytesPerQueue");

    // TODO
    //if (maxBytesPerQueue == -1)
    //{
    //    return (100000);
    //}
    return maxBytesPerQueue;
}


void BuffersModule::setUpFrameBuffers()
{
    this->frameBuffers.resize(this->numberSupportedPriorities, NULL);

   //configure buffers for ethernet messages
   for(int iteratorPriority = 0; iteratorPriority < this->numberSupportedPriorities; iteratorPriority++)
   {
       //instantiate new buffer with one priority and a defined size
       this->frameBuffers[iteratorPriority] = new FrameBuffer(iteratorPriority, this->maximumBufferSize);
   }
}


void BuffersModule::handleMessage(cMessage* message)
{
    if(message->isSelfMessage())    //sanity check
    {
        //TODO: ERROR, no internal messages to deal with
    }

    dealWithExternalMessage(message);
}


void BuffersModule::dealWithExternalMessage(cMessage* message)
{
    if(isMessageControlMessage(message) == true)
    {
        dealWithBufferControl(message);
    }
    else
    {
        dealWithEthernetFrame(message);
    }
}

bool BuffersModule::isMessageControlMessage(cMessage* message)
{
    cGate* arrivalGate = message->getArrivalGate();
    const char* gateName = arrivalGate->getName();
    bool controlMessage = strcmp(gateName, "control$i");

    //swap true and false with negation
    return !controlMessage;
}


void BuffersModule::dealWithBufferControl(cMessage* message)
{
    BufferControlMessage* bufferControlMessage = castMessageIntoBufferControlMessage(message);
    int messageInstruction = getBufferControlInstruction(bufferControlMessage);
    handleBufferControlInstruction(messageInstruction, bufferControlMessage);
}

void BuffersModule::dealWithEthernetFrame(cMessage* message)
{
    EthernetFrame* ethernetFrame = castMessageIntoEthernetFrame(message);

    int framePriority = ethernetFrame->getPriority();
    bool isEmpty = isFristArrivingFrameWithPriority(framePriority);
    storeEthernetFrameInBuffer(framePriority, ethernetFrame);

    if(isEmpty == true)
    {
        triggerUpdateMessageNewPriorityArrival();
    }
}

BufferControlMessage* BuffersModule::castMessageIntoBufferControlMessage(cMessage* message)
{
   BufferControlMessage* bufferControlMessage = check_and_cast<BufferControlMessage*>(message);

   if(bufferControlMessage == nullptr)
   {
       //TODO: Error Message
       EV <<("[BuffersModule] ERROR!");
       throw cRuntimeError("[BuffersModule] ERROR !");
   }

   return bufferControlMessage;
}


int BuffersModule::getBufferControlInstruction(BufferControlMessage* controlMessage)
{
    int messageInstruction = controlMessage->getMessageInformation();
    return messageInstruction;
}


void BuffersModule::handleBufferControlInstruction(int instruction, BufferControlMessage* controlMessage)
{
    switch(instruction)
    {
        case BUFFER_LEVEL_REQUEST:
            EV << "[BUFFER_LEVEL_REQUEST]\n";
            bufferControlLevelRequest();
            delete controlMessage;
            break;
        case SEND_AUTHORISATION:
            EV << "[SEND_AUTHORISATION]\n";
            sendEthernetFrameFromBuffer(controlMessage);
            delete controlMessage;
            break;
        default:
            //TODO: Throw Error-Message
            EV <<("[BuffersModule] ERROR!");
            throw cRuntimeError("[BuffersModule] ERROR !");
    }
}


void BuffersModule::bufferControlLevelRequest()
{
    std::vector<ethernetHeaderInformation*> informationPerBuffer = setUpBufferInformation();
    sendBufferControlMessage(informationPerBuffer, BUFFER_LEVEL_RESPONSE);
}


void BuffersModule::sendEthernetFrameFromBuffer(BufferControlMessage* controlMessage)
{
    int priority = controlMessage->getAuthorizedBuffer();
    EthernetFrame* frame = this->frameBuffers[priority]->popNextFrame();

    send(frame, "out");
}


void BuffersModule::triggerUpdateMessageNewPriorityArrival()
{
    std::vector<ethernetHeaderInformation*> informationPerBuffer = setUpBufferInformation();
    sendBufferControlMessage(informationPerBuffer, BUFFER_LEVEL_UPDATE);
}


EthernetFrame* BuffersModule::castMessageIntoEthernetFrame(cMessage* message)
{
    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*>(message);

    if(ethernetFrame == nullptr)
    {
        //TODO: Error Message
        EV <<("[BuffersModule] ERROR!");
        throw cRuntimeError("[BuffersModule] ERROR !");
    }

    return ethernetFrame;
}


bool BuffersModule::isFristArrivingFrameWithPriority(int priority)
{
    bool isEmpty = true;
    int bufferFrameFillLevel = this->frameBuffers[priority]->getFrameFillLevel();

    if(bufferFrameFillLevel > 0)
    {
        //at least one frame is stored  of this priority
        isEmpty = false;
    }

    return isEmpty;
}


void BuffersModule::storeEthernetFrameInBuffer(int priority, EthernetFrame* ethernetFrame)
{
    this->frameBuffers[priority]->pushFrameToBack(ethernetFrame);
}


std::vector<ethernetHeaderInformation*> BuffersModule::setUpBufferInformation()
{
    std::vector<ethernetHeaderInformation*> informationPerBuffer;
    informationPerBuffer.resize(this->numberSupportedPriorities);

    for(int iteratorPriority = 0; iteratorPriority < this->numberSupportedPriorities; iteratorPriority++)
    {
        informationPerBuffer[iteratorPriority] = getFrameBufferInformation(iteratorPriority);
    }
    return informationPerBuffer;
}


ethernetHeaderInformation* BuffersModule::getFrameBufferInformation(int priority)
{
    ethernetHeaderInformation* frameInformation = new ethernetHeaderInformation;

    int fillLevel = this->frameBuffers[priority]->getFrameFillLevel();
    int framePriority = this->frameBuffers[priority]->getFramePriority();
    int frameLength = 0;

    if(fillLevel != 0)
    {
        frameLength = this->frameBuffers[priority]->getNextFrameLength();;
    }

    frameInformation->payloadLength = frameLength;
    frameInformation->priority = framePriority;
    return frameInformation;
}


void BuffersModule::sendBufferControlMessage(std::vector<ethernetHeaderInformation*> bufferInformation,
        messageInformationConstant status)
{
    BufferControlMessage* bufferControlMessage = new BufferControlMessage();
    bufferControlMessage->setInformationPerBuffer(bufferInformation);
    bufferControlMessage->setMessageInformation(status);

    send(bufferControlMessage, "control$o");
}


void BuffersModule::deleteFramesInBuffers()
{
    // delete every frame that is stored in the buffers
    for(int iterator = 0; iterator < this->numberSupportedPriorities; iterator++)
    {
        // go through the queue until the level is empty
        while(this->frameBuffers[iterator]->getFrameFillLevel() != 0)
        {
            delete this->frameBuffers[iterator]->deleteFramesInBuffer();
        }
    }
}

void BuffersModule::deleteBuffers()
{
    // delete every created FrameBuffer
    for(int iteratorPriority = 0; iteratorPriority < this->numberSupportedPriorities; iteratorPriority++)
    {
        delete this->frameBuffers[iteratorPriority];
    }
}


