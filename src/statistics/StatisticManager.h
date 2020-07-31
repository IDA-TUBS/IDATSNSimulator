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

#ifndef SRC_STATISTICS_STATISTICMANAGER_H_
#define SRC_STATISTICS_STATISTICMANAGER_H_

#include <map>
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>



#include "FramesLostStatistics.h"
#include "ddsSampleLatencyStatistics.h"
#include "E2ELatencyStatistics.h"
#include "LinkTransmissionStatistics.h"
#include "TimeAwareShaperSegmentStatistic.h"

using namespace omnetpp;
/**
 * StatisticManager introduces an virtual module for statistic gathering.
 * It is necessary whenever we are gathering statistics for entities which does not have representation in form of an Omnet++ module.
 * For instance, DDS samples from specific reader/writer pairs, or latencies for particular stream.
 * Omnet++ does not support such statistics natively as framework was oriented towards average / performanced optimized results.
 */
class StatisticManager
{
public:
    /**
     * Returns the singleton instance of the collector,
     * which allows to gather statistics from different simulation modules in one object.
     */
    static StatisticManager& getInstance()
    {
        static StatisticManager instance;

        return instance;
    }
    /**
     * Desctructor, should stay empty
     */
    virtual ~StatisticManager();

    /**
     * Function to return an object storing the delay statistics.
     * The statistics are stored for End-2-End communication within the network.
     * If an object does not exist it declares a new one.
     * @return
     */
    E2ELatencyStatistics* getDelayStatistics();
    /**
     * Function to return an object storing the lost frames statistics.
     * If an object does not exist it declares a new one.
     * @return
     */
    FramesLostStatistics* getFramesLostStatistics();
    /**
     * Function to return an object storing the latency of whole DDS samples.
     * Note, that each sample may be constructed from multiple frames.
     * If an object does not exist it declares a new one.
     * @return
     */
    SampleLatencyStatistics* getSampleLatencyStatistics();

    /**
     * Function to return an object storing the transmission time of an Ethernet
     * frame from one module to the next module.
     *
     * @return
     */
    LinkTransmissionStatistics* getLinkTransmissionStatistics();

    /**
     * Function to return an object of the time aware shaper (TAS) segments. The
     * period of TAS is divided in four segments. For evaluation these segments
     * are represented as statistics.
     */
    TimeAwareShaperSegmentStatistic* getTimeAwareShaperStatistics();

    /**
     * finishes off all individual collections (see individual functions), frees memory and resets pointer
     */
    void virtual finishAll();

private:
    ///this is singleton, make constructor private
    StatisticManager();
    ///variable to indicate if running finish will be necessary
    bool hasFinishedAll = false;
    /// object to store E2E latencies
    E2ELatencyStatistics* pathDelayStats = 0;
    /// object to store lost frames statistics
    FramesLostStatistics* framesLostStats = 0;
    /// object to store DDS samples latencies
    SampleLatencyStatistics* sampleLatencyStats = 0;
    /// object to store link transmission statistics
    LinkTransmissionStatistics* linkTransmission = 0;
    /// object to store TAS statistics
    TimeAwareShaperSegmentStatistic* tsnTASStatistics;

};

#endif
