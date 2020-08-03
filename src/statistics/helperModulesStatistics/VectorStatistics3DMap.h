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

#ifndef SRC_STATISTICS_HELPERMODULESSTATISTICS_VECTORSTATISTICS3DMAP_H_
#define SRC_STATISTICS_HELPERMODULESSTATISTICS_VECTORSTATISTICS3DMAP_H_

#include <omnetpp.h>
#include <tuple>
#include <map>

using namespace omnetpp;
using std::pair;
using std::map;

class VectorStatistics3DMap
{
public:
    /**
     * Constructor of the class
     * Stores parameters which will be later used for the creation of the string which is used to identify results.
     * @param prefix
     * @param modulePrefix
     * @param submodulePrefix
     * @param valuePrefix
     */
    VectorStatistics3DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix, const char* valuePrefix);
    /**
     *  Destructor, nothing to do
     */
    virtual ~VectorStatistics3DMap();
    /**
     *  Finish function is used to clear & save the statistics at the end of simulation
     */
    void finish();
    /**
     *  Function stores the integer measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. bufferID
     * @param valueID - id of the stream/streams for which statistics has been taken e.g. priority, streamID
     * @param value - the measured value, must be an integer
     */
    void addInteger(int moduleID, int submoduleID, int valueID, int value);
    /**
     *  Function stores the integer measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. bufferID
     * @param valueID - id of the stream/streams for which statistics has been taken e.g. priority, streamID
     * @param value - the measured value, must be an integer
     */
    void accumulateInteger(int moduleID, int submoduleID, int valueID, int value);
    /**
     * Function firstly accumulates the value to the last one and later stores the integer measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. bufferID
     * @param valueID - id of the stream/streams for which statistics has been taken e.g. priority, streamID
     * @param value - the measured value, must be an simtime_t
     */
    void addLatency(int moduleID, int submoduleID, int valueID, simtime_t value);
    /**
      *  Function stores the integer measurement results into the statistics vector
      * @param moduleID - id of the module where the measurement has been taken
      * @param submoduleID - id of the module for which the statistics has been taken e.g. bufferID
      * @param valueID - id of the stream/streams for which statistics has been taken e.g. priority, streamID
      * @param valueIDStr - substitutes the valueID if defined
      * @param value - the measured value, must be an simtime_t
      */
    void addLatency(int moduleID, int submoduleID, int valueID, const std::string& valueIDStr, simtime_t value);

private:
    static const std::string empty;

    ///prefix is a string which stores information allowing to identify the type of statistics which is gathered e.g. E2E latency, packet drop etc.
    const char* prefix = 0;
    ///modulePrefix is a string which denotes the module where statistics are gathered e.g. sink with a selected ID
    const char* modulePrefix = 0;
    ///submodule is a string which prefix denotes for which component the statistics are gathered, it could be streamID or buffer ID
    const char* submodulePrefix = 0;
    ///value prefix is a string allowing to identify the type of stored data
    const char* valuePrefix = 0;
    ///histogram containing the stored results, thre dimmensional

    map<int, map<int, map<int, pair<cOutVector*, long>>*>*>* storedMeasurements;

    pair<cOutVector*, long>& getEntry(int moduleID, int submoduleID, int valueID);
    pair<cOutVector*, long>& getEntry(int moduleID, int submoduleID, int valueID, const std::string& valueIDStr);
};

#endif /* SRC_STATISTICS_HELPERMODULESSTATISTICS_VECTORSTATISTICS3DMAP_H_ */
