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

#ifndef ETHERNET_CLASSES_COLLECTION_PATHDELAYSTATISTICS_H_
#define ETHERNET_CLASSES_COLLECTION_PATHDELAYSTATISTICS_H_

#include <omnetpp.h>
#include <string>

#include "../common/GlobalMacros.h"
#include "../messagetype/EthernetFrame_m.h"
#include "helperModulesStatistics/HistogramStatistics2DMap.h"
#include "helperModulesStatistics/VectorStatistics2DMap.h"

using namespace omnetpp;
/**
 * This class is gathering E2E latency statistics per streamID
 * As streams are virtual entities (no module) they must be gathered by StatisticManager.
 */
class E2ELatencyStatistics
{
public:

    /**
     * default constructor, nothing to do
     */
    E2ELatencyStatistics();
    /**
     * default destructor, nothing to do
     */
    virtual ~E2ELatencyStatistics();

    /**
     * stores the end-to-end delay of at the receiver of the stream ID
     *
     * @param receiverID the end node ID (must be unique)
     * @param globalStreamID the stream ID of the received message
     * @param delay the end-to-end delay (receive time - start time)
     *
     * @return 1 if success, see error codes within code
     */
    int addE2ELatency(int receiverID, int globalStreamID, simtime_t delay);

    /**
     * Finishes statistics gathering for End-2-End latencies
     * Vectors will be deleted
     * Histograms: are firstly recorded (save to file) and next deleted
     */
    void finish();
private:

    // vector which is 2D map for storing E2E latency statistics
    VectorStatistics2DMap* vectorE2EStatistics = 0;
    // histogram which is 2D map for storing E2E latency statistics
    HistogramStatistics2DMap* histogramE2Estatistics = 0;
};

#endif /* ETHERNET_CLASSES_COLLECTION_PATHDELAYSTATISTICS_H_ */
