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

#include "HistogramStatistics3DMap.h"

using std::string;
using std::stringstream;

const string HistogramStatistics3DMap::empty("");

HistogramStatistics3DMap::HistogramStatistics3DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix, const char* valuePrefix)
{
    this->prefix = prefix;
    this->modulePrefix = modulePrefix;
    this->submodulePrefix = submodulePrefix;
    this->valuePrefix = valuePrefix;
    this->storedMeasurements = 0;
}

HistogramStatistics3DMap::~HistogramStatistics3DMap()
{
    // nothing to do
}

void HistogramStatistics3DMap::addInteger(int moduleID, int submoduleID, int valueID, int value)
{
    getEntry(moduleID, submoduleID, valueID)->collect(value);
}

void HistogramStatistics3DMap::addLatency(int moduleID, int submoduleID, int valueID, simtime_t value)
{
    getEntry(moduleID, submoduleID, valueID)->collect(value);
}

void HistogramStatistics3DMap::addLatency(int moduleID, int submoduleID, int valueID, const string& valueIDStr, simtime_t value)
{
    getEntry(moduleID, submoduleID, valueID, valueIDStr)->collect(value);
}

cDoubleHistogram* HistogramStatistics3DMap::getEntry(int moduleID, int submoduleID, int valueID)
{
    return getEntry(moduleID, submoduleID, valueID, empty);
}

cDoubleHistogram* HistogramStatistics3DMap::getEntry(int moduleID, int submoduleID, int valueID, const string& valueIDStr)
{
    if (this->storedMeasurements == 0)
    {
        this->storedMeasurements = new std::map<int, std::map<int, std::map<int, cDoubleHistogram*>*>*>();
    }

    if (this->storedMeasurements->count(moduleID) == 0)
    {
        (*this->storedMeasurements)[moduleID] = new std::map<int, std::map<int, cDoubleHistogram*>*>();
    }

    std::map<int, std::map<int, cDoubleHistogram*>*>* moduleMap = (*this->storedMeasurements)[moduleID];

    if (moduleMap->count(submoduleID) == 0)
    {
        (*moduleMap)[submoduleID] = new std::map<int, cDoubleHistogram*>();
    }

    std::map<int, cDoubleHistogram*>* valueMap = (*moduleMap)[submoduleID];

    if (valueMap->count(valueID) == 0)
    {
        stringstream ss;
        ss << "[H]" << prefix << ":" << modulePrefix << "=" << moduleID << "_" << submodulePrefix << "=" << submoduleID
                << "_" << valuePrefix << "=";
        if (empty == valueIDStr)
        {
            ss << valueID;
        }
        else
        {
            ss << valueIDStr;
        }
        cDoubleHistogram* temporalVariable = new cDoubleHistogram(ss.str().c_str());
        (*valueMap)[valueID] = temporalVariable;
    }

    return (*valueMap)[valueID];
}


void HistogramStatistics3DMap::finish()
{
    if (this->storedMeasurements)
    {
        for (auto module = this->storedMeasurements->begin(); module != this->storedMeasurements->end(); ++module)
        {
            std::map<int, std::map<int, cDoubleHistogram*>*>* portHistogram = module->second;

            for (auto submodule = portHistogram->begin(); submodule != portHistogram->end(); ++submodule)
            {
                std::map<int, cDoubleHistogram*>* prioHistogram = submodule->second;

                for (auto valueID = prioHistogram->begin(); valueID != prioHistogram->end(); ++valueID)
                {
                    valueID->second->recordAs(valueID->second->getName());
                    delete (valueID->second);
                }
                delete prioHistogram;
            }
            delete portHistogram;
        }
        delete this->storedMeasurements;
        this->storedMeasurements = 0;
    }

}
