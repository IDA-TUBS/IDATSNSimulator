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

#include <sstream>
#include "VectorStatistics3DMap.h"

using std::string;
using std::stringstream;

const string VectorStatistics3DMap::empty("");

VectorStatistics3DMap::VectorStatistics3DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix, const char* valuePrefix)
{
    this->prefix = prefix;
    this->modulePrefix = modulePrefix;
    this->submodulePrefix = submodulePrefix;
    this->valuePrefix = valuePrefix;
    this->storedMeasurements = nullptr;
}

VectorStatistics3DMap::~VectorStatistics3DMap()
{
    // nothing to be done
}

void VectorStatistics3DMap::addInteger(int moduleID, int submoduleID, int valueID, int value)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, valueID);
    storedEntry.first->record(value);
    storedEntry.second = storedEntry.second + value;
}


void VectorStatistics3DMap::accumulateInteger(int moduleID, int submoduleID, int valueID, int value)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, valueID);
    storedEntry.second = storedEntry.second + value;
    storedEntry.first->record(storedEntry.second);
}

void VectorStatistics3DMap::addLatency(int moduleID, int submoduleID, int valueID, simtime_t value)
{
    pair<cOutVector*, long> & entry_ref = getEntry(moduleID, submoduleID, valueID);
    entry_ref.first->record(value);
}

void VectorStatistics3DMap::addLatency(int moduleID, int submoduleID, int valueID, const string& valueIDStr, simtime_t value)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, valueID, valueIDStr);
    storedEntry.first->record(value);
}

pair<cOutVector*, long>& VectorStatistics3DMap::getEntry(int moduleID, int submoduleID, int valueID)
{
    return getEntry(moduleID, submoduleID, valueID, empty);
}

pair<cOutVector*, long>& VectorStatistics3DMap::getEntry(int moduleID, int submoduleID, int valueID,
        const string& valueIDStr)
{
    if (this->storedMeasurements == 0)
    {
        this->storedMeasurements = new map<int, map<int, map<int, pair<cOutVector*, long>>*>*>();
    }
    if (this->storedMeasurements->count(moduleID) == 0)
    {
        (*this->storedMeasurements)[moduleID] = new map<int, map<int, pair<cOutVector*, long>>*>();
    }

    auto moduleMap = (*this->storedMeasurements)[moduleID];
    if (moduleMap->count(submoduleID) == 0)
    {
        (*moduleMap)[submoduleID] = new map<int, pair<cOutVector*, long>>();
    }

    auto valueMap = (*moduleMap)[submoduleID];
    if (valueMap->count(valueID) == 0)
    {
        stringstream ss;
        ss << prefix << ":" << modulePrefix << "=" << moduleID << "_" << submodulePrefix << "=" << submoduleID << "_"
                << valuePrefix << "=";
        if (empty == valueIDStr)
        {
            ss << valueID;
        }
        else
        {
            ss << valueIDStr;
        }
        (*valueMap)[valueID] = pair<cOutVector*, long>(new cOutVector(ss.str().c_str()), 0);
    }
    pair<cOutVector*, long> & storedEntry = (*valueMap)[valueID];
    return storedEntry;
}

void VectorStatistics3DMap::finish()
{
    if (this->storedMeasurements)
    {
        for (auto module = this->storedMeasurements->begin(); module != this->storedMeasurements->end(); ++module)
        {
            auto moduleVector = module->second;
            for (auto submodule = moduleVector->begin(); submodule != moduleVector->end(); ++submodule)
            {
                auto submoduleVector = submodule->second;
                for (auto value = submoduleVector->begin(); value != submoduleVector->end(); ++value)
                {
                    delete value->second.first;
                }
                delete submoduleVector;
            }
            delete moduleVector;
        }
        delete this->storedMeasurements;
        this->storedMeasurements = 0;
    }

}
