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

#ifndef SWITCH_EGRESSPORT_FRAMEBUFFER_H_
#define SWITCH_EGRESSPORT_FRAMEBUFFER_H_

#include <omnetpp.h>
#include <iostream>
#include "../../messagetype/EthernetFrame_m.h"

using namespace omnetpp;

/**
 * Enum for updating the FrameBuffer. ADDED_FRAME indicates
 * that a new frame is stored into the buffer. REMOVED_FRAME
 * indicates that a frame is deleted.
 */
enum bufferUpdate
{
    ADDED_FRAME = 0,
    REMOVED_FRAME = 1,
};

/**
 * Class FrameBuffer is one buffer in which are frames (e.g. Ethernet frames)
 * are stored. Only frames of the same priority are in one buffer.
 * The Buffer size (capacity in byte) depends on the module BufferModuls.ned
 *
 * FrameBuffer is similar to a queue. New arriving frames are stored at the end of
 * the buffers. Always the first element is selected for transmission.
 */
class FrameBuffer
{
public:
    /**
     * Constructor of FrameBuffer. By creating a new object FrameBuffer
     * it is necessary to set the priority and the size (Byte) of the buffer
     *
     * @param priority integer value of the FrameBuffer priority depending on frame priority
     * @param maximumBufferSize integer value that defines the buffer size (set per default to 4096 byte)
     */
    FrameBuffer(int priority, int maximumBufferSize);

    /**
     * Destructor of FrameBuffer -> empty, nothing to do here
     */
    virtual ~FrameBuffer();

    /**
     * Get the priority of a FrameBuffer and thus the stored frame priority
     *
     * @return The integer value priority of a FrameBuffer
     */
    int getFramePriority();

    /**
     * Get the number of frames stored in the FrameBuffer
     *
     * @return The integer value number of frames stored in the buffer
     */
    int getFrameFillLevel();

    /**
     * Get the frame length of the first frame in the buffer
     *
     * @return The length of the first frame in the buffer (in byte)
     */
    int getNextFrameLength();

    /**
     * Add a new frame into the FrameBuffer. Frame buffer is similar
     * to a queue. Frames can only be added at the end of the buffer
     *
     * @param frame Pointer of EthernetFrame that is added to FrameBuffer
     */
    void pushFrameToBack(EthernetFrame* frame);

    /**
     * Remove the first frame of the FrameBuffer. To simulate
     * "first in, first out" (FIFO) frames are only be removed
     * from the front of the buffer
     *
     * @return Pointer to first EthernetFrame in Buffer.
     */
    EthernetFrame* popNextFrame();

    /**
     * This function is called by destructor ~BuffersModule()  to delete
     * every remaining frame in the buffer.
     *
     * After finishing the simulation every remaining Ethernet frame
     * in a buffer must be deleted. In contrast to popNextFrame() no
     * statistics are collected here. Due to the statistics
     * popNextFrame() is improper for the destructor.
     *
     * @return Pointer to the first stored Ethernet frame in buffer.
     */
    EthernetFrame* deleteFramesInBuffer();

private:
    int framePriority;
    int bufferSize;
    int frameNumberInBuffer;
    int storedBytesInBuffer;
    int droppedFramesInBuffer;
    int droppedBytesInBuffer;


    cOutVector frameNumberVector;
    cOutVector storedBytesVector;
    cOutVector droppedFramesVector;
    cOutVector droppedBytesVector;


    /**
     * One FrameBuffer is implemented as list but
     * acts like a queue (first in, first out)
     */
    std::list<EthernetFrame*> frameBuffer;

    /**
     * Update the number of free space in buffer.
     *
     * By adding or removing a frame the frame buffer size has
     * to be updated. The frame length of a new arrived frame
     * decreases the buffer size. The frame length of a removed
     * frame increases the buffer size
     *
     * @param status Specifies whether a frame is added or removed
     * @param frameLength integer value indicating the added/removed frame length
     *
     * @return integer value of the updated buffer size (in byte)
     */
    int updateFreeBufferSize(bufferUpdate status, int frameLength);

    /**
     * checks if there is enough space in the buffer to store
     * a frame
     *
     * @param frameLength Integer value, indicates the length of the frame to
     *                    be stored
     *
     * @return returns true if the bufferSize is larger than frame length
     *         otherwise false
     */
    bool freeSpaceInBuffer(int frameLenght);
};

#endif /* SWITCH_EGRESSPORT_FRAMEBUFFER_H_ */
