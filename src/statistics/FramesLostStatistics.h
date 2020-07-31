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


#ifndef ETHERNET_COLLECTION_FRAMESLOSTSTATISTICS_H_
#define ETHERNET_COLLECTION_FRAMESLOSTSTATISTICS_H_

#include <omnetpp.h>

using namespace omnetpp;
#include "helperModulesStatistics/HistogramStatistics2DMap.h"
#include "helperModulesStatistics/VectorStatistics2DMap.h"

/**
 * This class is gathering statistics for the frame loss per streamID
 * As streams are virtual entities (no module) they must be gathered by StatisticManager.
 */
class FramesLostStatistics
{
public:

    /**
     * default constructor, nothing to do
     */
    FramesLostStatistics();
    /**
     * default destructor, nothing to do
     */
    virtual ~FramesLostStatistics();
    void finish();


    /**
     * stores the number of lost frames at the receiver by the stream with streamID
     *
     * @param receiverID the end node ID (must be unique)
     * @param globalStreamID the stream ID of the received message
     * @param counter number of lost frames
     */
    void addLostFrame(int receiverID, int globalStreamID, int counter);
    void addLostFrameTrace(int receiverID, int globalStreamID, int counter);

private:
    // vector which is 2D map for storing lost frames statistics
    VectorStatistics2DMap* vectorLostFramesPerStream = 0;
    // vector which is 2D map for storing lost frames statistics
     VectorStatistics2DMap* vectorLostFramesTracePerStream = 0;
    // histogram which is 2D map for storing lost frames statistics
    HistogramStatistics2DMap* histogramLostFramesPerStream = 0;
};

#endif /* ETHERNET_COLLECTION_FRAMESLOSTSTATISTICS_H_ */
