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

#include "HistogramStatistics2DMap.h"

/**
 * creates the statistic collector
 * @param prefix what name should be addded to the statistic Histogram?
 * @param is this collection using the stream ID or the stream priority
 */
HistogramStatistics2DMap::HistogramStatistics2DMap(const char* prefix, const char* modulePrefix, const char* submodulePrefix)
{
    this->prefix = prefix;
    this->modulePrefix = modulePrefix;
    this->submodulePrefix = submodulePrefix;

}

HistogramStatistics2DMap::~HistogramStatistics2DMap()
{
    // should have been cleaned in finish already
}

/*
 * stores the given value associated with the module(switchMAC) and submodule(portID)
 */
void HistogramStatistics2DMap::addIntegerResult(int moduleID, int submoduleID, int value)
{
    if (this->storedMeasurements == 0)
    {
        this->storedMeasurements = new std::map<int, std::map<int, cDoubleHistogram*>*>();
    }

    if (this->storedMeasurements->count(moduleID) == 0)
    {
        (*this->storedMeasurements)[moduleID] = new std::map<int, cDoubleHistogram*>();
    }

    //the map where all the entries for this switch/gw/end node are stored
    std::map<int, cDoubleHistogram*>* moduleMap = (*this->storedMeasurements)[moduleID];

    if (moduleMap->count(submoduleID) == 0)
    {
        char buffer[80];    //write the name here
        sprintf(buffer, "[H]%s:%s=%d_%s=%d", this->prefix, this->modulePrefix, moduleID, this->submodulePrefix, submoduleID);
        cDoubleHistogram* temporalVariable = new cDoubleHistogram();
        temporalVariable->setName(buffer);
        (*moduleMap)[submoduleID] = temporalVariable;
    }

    //store the value
    cDoubleHistogram* v = (*moduleMap)[submoduleID];
    v->collect(value);

}

/**
 * stores the given time associated with the switchMAC, portID and priority
 */
void HistogramStatistics2DMap::addLatency(int moduleID, int submoduleID, simtime_t value)
{
    if (this->storedMeasurements == 0)
    {
        this->storedMeasurements = new std::map<int, std::map<int, cDoubleHistogram*>*>();
    }

    if (this->storedMeasurements->count(moduleID) == 0)
    {
        (*this->storedMeasurements)[moduleID] = new std::map<int, cDoubleHistogram*>();
    }

    //the map where all the entries for this switch/gw/end node are stored
    std::map<int, cDoubleHistogram*>* moduleMap = (*this->storedMeasurements)[moduleID];

    if (moduleMap->count(submoduleID) == 0)
    {
        char buffer[80];    //write the name here
        sprintf(buffer, "[H]%s:%s=%d_%s=%d", this->prefix, this->modulePrefix, moduleID, this->submodulePrefix, submoduleID);
        cDoubleHistogram* temporalVariable = new cDoubleHistogram();
        temporalVariable->setName(buffer);
        (*moduleMap)[submoduleID] = temporalVariable;
    }

    //store the value
    cDoubleHistogram* v = (*moduleMap)[submoduleID];
    v->collect(value);
}

/**
 * this calls recordAs on the histograms with the name given in add_simtime/add_int
 * if there are any values stored, delete them (free memory and reset pointers)
 */
void HistogramStatistics2DMap::finish()
{
    //only finish if not yet done
    if (this->storedMeasurements)
    {
        //iterate over all mac addresses which used a Histogram
        for (auto measurement = this->storedMeasurements->begin(); measurement != this->storedMeasurements->end(); ++measurement)
        {
            std::map<int, cDoubleHistogram*>* submHistogram = measurement->second;

            //iterate over all ports this mac address used
            for (auto submeasurement = submHistogram->begin(); submeasurement != submHistogram->end(); ++submeasurement)
            {
                //drop all elements from this map
                submeasurement->second->recordAs(submeasurement->second->getName());
                delete submeasurement->second;

            }

            //drop all elements from this map
            delete submHistogram;
        }

        //forget everything
        delete this->storedMeasurements;
        this->storedMeasurements = 0;
    }

}
