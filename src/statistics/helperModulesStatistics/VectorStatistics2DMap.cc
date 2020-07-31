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

#include "VectorStatistics2DMap.h"

using std::pair;
using std::map;


VectorStatistics2DMap::VectorStatistics2DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix, const char* submoduleProperty)
{
    this->prefix = prefix;
    this->modulePrefix = modulePrefix;
    this->submodulePrefix = submodulePrefix;
    this->submoduleProperty = submoduleProperty;
}

VectorStatistics2DMap::~VectorStatistics2DMap()
{
    // nothing to do
}


void VectorStatistics2DMap::addInteger(int moduleID, int submoduleID, int value)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, 0);
    storedEntry.first->record(value);
    storedEntry.second = storedEntry.second + value;
}

void VectorStatistics2DMap::addLatency(int moduleID, int submoduleID, simtime_t value, int priority)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, priority);
    storedEntry.first->record(value);
}

void VectorStatistics2DMap::accumulateInteger(int moduleID, int submoduleID, int value)
{
    pair<cOutVector*, long> & storedEntry = getEntry(moduleID, submoduleID, 0);
    storedEntry.second = storedEntry.second + value;
    storedEntry.first->record(storedEntry.second);
}

pair<cOutVector*, long>& VectorStatistics2DMap::getEntry(int moduleID, int submoduleID, int priority)
{
    if (this->storedMeasurements == 0)
    {
        this->storedMeasurements = new std::map<int, std::map<int, pair<cOutVector*, long>>*>();
    }

    if (this->storedMeasurements->count(moduleID) == 0)
    {
        (*this->storedMeasurements)[moduleID] = new std::map<int, pair<cOutVector*, long>>();
    }

    auto moduleMap = (*this->storedMeasurements)[moduleID];

    if (moduleMap->count(submoduleID) == 0)
    {
        char buffer[80];
        sprintf(buffer, "%s:%s=%d_%s=%d:%s=%d", this->prefix, this->modulePrefix, moduleID, this->submodulePrefix, submoduleID, this->submoduleProperty, priority);
        (*moduleMap)[submoduleID] = pair<cOutVector*, long>(new cOutVector(buffer), 0);
    }

    pair<cOutVector*, long> & storedEntry = (*moduleMap)[submoduleID];
    return storedEntry;
}

void VectorStatistics2DMap::finish()
{
    if (this->storedMeasurements)
    {
        for (auto module = this->storedMeasurements->begin(); module != this->storedMeasurements->end(); ++module)
        {
            auto submVector = module->second;

            for (auto submodule = submVector->begin(); submodule != submVector->end(); ++submodule)
            {
                cOutVector* prioVector = submodule->second.first;

                delete prioVector;
            }
            delete submVector;
        }

        delete this->storedMeasurements;
        this->storedMeasurements = 0;
    }

}
