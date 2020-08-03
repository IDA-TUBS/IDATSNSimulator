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

#ifndef SRC_STATISTICS_DDSSAMPLELATENCYSTATISTICS_H_
#define SRC_STATISTICS_DDSSAMPLELATENCYSTATISTICS_H_

#include <omnetpp.h>

#include "helperModulesStatistics/HistogramStatistics3DMap.h"
#include "helperModulesStatistics/VectorStatistics3DMap.h"

/**
 * This class is gathering E2E latency statistics per DDS samples
 * As DDS samples are virtual entities (no module) they must be gathered by StatisticManager.
 */
class SampleLatencyStatistics
{
public:
    /**
     * default constructor, nothing to do
     */
    SampleLatencyStatistics();
    /**
     * default destructor, nothing to do
     */
    virtual ~SampleLatencyStatistics();
    /**
     * stores the end to end statistics for the samples
     * @param participantId - DDS participant ID
     * @param readerId - DDS reader ID
     * @param globalStreamId - id of the stream
     * @param delay - end to end latency for the sample
     */
    void addMessage(int participantId, int readerId, int globalStreamId, simtime_t delay);
    /**
     * Stores the results into the vectors
     * Helper function
     * @param latency
     * @param participantId
     * @param readerId
     * @param globalStreamId
     */
    void report(const omnetpp::simtime_t& latency, int participantId, int readerId, int globalStreamId);
    /**
     * Finishes statistics gathering for End-2-End latencies
     * Vectors will be deleted
     * Histograms: are firstly recorded (save to file) and next deleted
     */
    void finish();

private:
    // vector object which is 3D map for storing E2E latency statistics for DDS samples
    VectorStatistics3DMap* sampleLatencyStatisticsVector = nullptr;
    // histogram object which is 3D map for storing E2E latency statistics for DDS samples
    HistogramStatistics3DMap* sampleLatencyStatisticsHistogram = nullptr;

};

#endif /* SRC_STATISTICS_DDSSAMPLELATENCYSTATISTICS_H_ */
