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


#ifndef SRC_STATISTICS_HELPERMODULESSTATISTICS_HISTOGRAMSTATISTICS2DMAP_H_
#define SRC_STATISTICS_HELPERMODULESSTATISTICS_HISTOGRAMSTATISTICS2DMAP_H_

#include <omnetpp.h>

#include "../../common/GlobalMacros.h"

using namespace omnetpp;

/**
 * Helper class used for storing the histogram statistics
 * for objects which do not have physical modules e.g. streams or DDS samples
 * with two parameters describing each entry e.g. moduleID and submoduleID
 */
class HistogramStatistics2DMap
{
public:
    /**
     * Constructor of the class
     * Stores parameters which will be later used for the creation of the string which is used to identify results.
     * @param prefix
     * @param modulePrefix
     * @param submodulePrefix
     */
    HistogramStatistics2DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix);
    /**
     *  Destructor, nothing to do
     */
    virtual ~HistogramStatistics2DMap();
    /**
     *  Finish function is used to clear & save the statistics at the end of simulation
     */
    void finish();
    /**
     * Function stores the integer measurement results into the statistics histogram
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. streamID
     * @param value - the measured value, must be an integer
     */
    void addIntegerResult(int moduleID, int submoduleID, int value);
    /**
     * Function stores the simulation latency measurement results into the statistics histogram
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. streamID
     * @param value - the measured value, must be an simtime_t
     */
    void addLatency(int moduleID, int submpduleID, simtime_t value);

private:
    ///histogram containing the stored results
    std::map<int, std::map<int, cDoubleHistogram*>*>* storedMeasurements= 0;

    ///prefix is a string which stores information allowing to indentify the type of statistics which is gathered e.g. E2E latency, packet drop etc.
    const char* prefix;
    ///modulePrefix is a string which denotes the module where statistics are gathered e.g. sink with a selected ID
    const char* modulePrefix;
    ///submodule is a string which prefix denotes for which component the statistics are gathered, it could be streamID or buffer ID
    const char* submodulePrefix;
    ///value prefix is a string allowing to identify the type of stored data
    const char* valuePrefix;

};

#endif /* SRC_STATISTICS_HELPERMODULESSTATISTICS_HISTOGRAMSTATISTICS2DMAP_H_ */
