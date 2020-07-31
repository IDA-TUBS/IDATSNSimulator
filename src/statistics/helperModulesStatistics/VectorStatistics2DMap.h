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

#ifndef ETHERNET_CLASSES_COLLECTION_COLLECTIONS_VECTORSTATISTICS2DMAP_H_
#define ETHERNET_CLASSES_COLLECTION_COLLECTIONS_VECTORSTATISTICS2DMAP_H_

#include <omnetpp.h>
#include <tuple>
#include "../../common/GlobalMacros.h"

using namespace omnetpp;

/**
 * Helper class used for storing the vector statistics
 * for objects which do not have physical modules e.g. streams or DDS samples
 * with two parameters describing each entry e.g. moduleID and submoduleID
 */
class VectorStatistics2DMap
{
public:

    /**
     * Constructor of the class
     * Generates the string using the parameter variables which is later used to describe/identif measurement results
     * @param prefix
     * @param module_fix
     * @param submodule_fix
     */
    VectorStatistics2DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix, const char* submoduleProperty);
    /**
     * Destructor of the class, nothing to do here
     */
    virtual ~VectorStatistics2DMap();
    /**
     *  Finish function is used to save the statistics at the end of simulation
     */
    void finish();
    /**
     * Function stores the integer measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. streamID
     * @param value - the measured value, must be an integer
     */
    void addInteger(int moduleID, int submoduleID, int value);
    /**
     * Function firstly accumulates the value to the last one and later stores the integer measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. streamID
     * @param value - the measured value, must be an integer
     */
    void accumulateInteger(int moduleID, int submoduleID, int value);
    /**
     * Function stores the simulation latency measurement results into the statistics vector
     * @param moduleID - id of the module where the measurement has been taken
     * @param submoduleID - id of the module for which the statistics has been taken e.g. streamID
     * @param value - the measured value, must be an simtime_t
     */
    void addLatency(int moduleID, int submpduleID, simtime_t value, int priority);

private:
    ///vector containing the stored results
    std::map<int, std::map<int, std::pair<cOutVector*, long>>*>* storedMeasurements = 0;

    ///prefix is a string which stores information allowing to indentify the type of statistics which is gathered e.g. E2E latency, packet drop etc.
    const char* prefix;
    ///modulePrefix is a string which denotes the module where statistics are gathered e.g. sink with a selected ID
    const char* modulePrefix;
    ///submodule is a string which prefix denotes for which component the statistics are gathered, it could be streamID or buffer ID
    const char* submodulePrefix;
    ///value prefix is a string allowing to identify the type of stored data
    const char* valuePrefix;
    //submoduleProperty is a string allowing to add a detailed description the type of stored data
    const char* submoduleProperty;

    /**
     * Function which returns an entry identified by parameters
     * @param module_ID - module where measurement has been takes e.g. bufferID
     * @param submodule_ID - module for which measurmenet has been take e.g. streamID
     * @return
     */
    std::pair<cOutVector*, long>& getEntry(int module_ID, int submodule_ID, int priority);
};

#endif /* ETHERNET_CLASSES_COLLECTION_COLLECTIONS_VECTORSTATISTICS2DMAP_H_ */
