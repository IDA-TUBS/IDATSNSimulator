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

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int priority, int maximumBufferSize)
{
    //set the buffer priority (equal to frame priority)
    this->framePriority = priority;

    //set the buffer size (in byte)
    this->bufferSize = maximumBufferSize;

    //initial buffer, no frames are stored at the beginning
    this->frameNumberInBuffer = 0;

    //initial buffer, no bytes are stored
    this->storedBytesInBuffer = 0;

    //initial buffer, no frames are dropped
    this->droppedFramesInBuffer = 0;

    //initial buffer, no bytes are dropped
    this->droppedBytesInBuffer = 0;

    //statistics vector for collecting the number of frames stored in the buffer for a given priority
    char name[32];
    sprintf(name, "frame-number-prio-%d", priority);
    frameNumberVector.setName(name);

    //statistics vector for collecting the amount of stored bytes in the buffer for a given priority
    sprintf(name, "stored-bytes-prio-%d", priority);
    this->storedBytesVector.setName(name);

    //statistics vector for collecting the amount of dropped frames in the buffer for a given priority
    sprintf(name, "dropped-frames-prio-%d", priority);
    this->droppedFramesVector.setName(name);

    //statistics vector for collecting the amount of dropped frames in the buffer for a given priority
    sprintf(name, "dropped-bytes-prio-%d", priority);
    this->droppedBytesVector.setName(name);
}


FrameBuffer::~FrameBuffer()
{
    // Auto-generated destructor stub
}


int FrameBuffer::getFramePriority()
{
    return this->framePriority;
}


int FrameBuffer::getFrameFillLevel()
{
    return this->frameNumberInBuffer;
}


int FrameBuffer::getNextFrameLength()
{
    int frameLength = 0;

    //read the first frame in buffer without deleting it
    EthernetFrame* frame = this->frameBuffer.front();

    //frame length is equal to frame PayloadSize
    frameLength = frame->getPayloadSize();

    return frameLength;
}


void FrameBuffer::pushFrameToBack(EthernetFrame* frame)
{
    int frameLength = frame->getPayloadSize();



    bool freeSpace = freeSpaceInBuffer(frameLength);

    //first check, if there is space to store the frame in buffer
    if(!freeSpace)
    {
        this->droppedFramesInBuffer++;
        droppedFramesVector.record(this->droppedFramesInBuffer);
        this->droppedBytesInBuffer = this->droppedBytesInBuffer + frameLength;
        droppedBytesVector.record(this->droppedBytesInBuffer);
        //drop frame
        delete frame;
    }
    else{
        //buffer has to be updated because of adding a new frame
        this->bufferSize = updateFreeBufferSize(ADDED_FRAME, frameLength);

        //store frame in buffer
        this->frameBuffer.push_back(frame);

        //increment the number of frames in buffer
        this->frameNumberInBuffer = this->frameNumberInBuffer + 1;
        this->storedBytesInBuffer = this->storedBytesInBuffer + frameLength;

        frameNumberVector.record(this->frameNumberInBuffer);
        storedBytesVector.record(this->storedBytesInBuffer);
    }
    
    
}


EthernetFrame* FrameBuffer::popNextFrame()
{
    int frameNumber = getFrameFillLevel();

    if(frameNumber <= 0) //sanity check, is a frame stored in buffer?
    {
        EV << "ERROR in [FrameBuffer]: no frames are stored in buffer with priority: "
                << this->framePriority << ". \n";
        throw cRuntimeError("[FrameBuffer] ERROR in [FrameBuffer] no frames are stored in buffer!");
    }

    // get the first frame in frameBuffer
    EthernetFrame* frame = this->frameBuffer.front();
    this->frameBuffer.pop_front();

    //frame length is equal to payload size
    int frameLength = frame->getPayloadSize();

    //update buffer size because of removing first frame
    this->bufferSize = updateFreeBufferSize(REMOVED_FRAME, frameLength);
    //statistics collection
    this->frameNumberInBuffer = this->frameNumberInBuffer-1;
    this->storedBytesInBuffer = this->storedBytesInBuffer - frameLength;
    frameNumberVector.record(this->frameNumberInBuffer);
    storedBytesVector.record(this->storedBytesInBuffer);

    return frame;
}


bool FrameBuffer::freeSpaceInBuffer(int frameLength)
{
    if(this->bufferSize < frameLength)
    {
        return false;
    }
    
    return true;
}


int FrameBuffer::updateFreeBufferSize(bufferUpdate status, int frameLength)
{
    //current size of the buffer corresponds to bufferSize
    int currentBufferSize = this->bufferSize;

    //new variable for updated buffer size
    int updatedBufferSize = 0;

    if(status == ADDED_FRAME) //a frame is added into the buffer
    {
        //buffer size decreases by frame length
        updatedBufferSize = currentBufferSize - frameLength;
    }
    else if (status == REMOVED_FRAME) //a frame is removed from the buffer
    {
        //buffer size increases by frame length
        updatedBufferSize = currentBufferSize + frameLength;
    }

    return updatedBufferSize;
}

EthernetFrame* FrameBuffer::deleteFramesInBuffer()
{
    int frameNumber = getFrameFillLevel();

    if(frameNumber <= 0) //sanity check, is a frame stored in buffer?
    {
        EV << "ERROR in [FrameBuffer]: no frames are stored in buffer with priority: "
                << this->framePriority << ". \n";
        throw cRuntimeError("[FrameBuffer] ERROR in [FrameBuffer] no frames are stored in buffer!");
    }

    // get the first frame in frameBuffer
    EthernetFrame* frame = this->frameBuffer.front();
    this->frameBuffer.pop_front();
    this->frameNumberInBuffer = this->frameNumberInBuffer-1;

    return frame;
}
