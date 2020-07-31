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

#ifndef SRC_STATISTICS_LINKTRANSMISSIONSTATISTICS_H_
#define SRC_STATISTICS_LINKTRANSMISSIONSTATISTICS_H_

#include <omnetpp.h>
#include <map>

#include "helperModulesStatistics/VectorStatistics3DMap.h"

using namespace omnetpp;

/**
 * The statistic LinkTransmissionStatistic records the transmission time
 * from one module to the next module.
 */
class LinkTransmissionStatistics
{
public:
    /**
     * default constructor, nothing to do here
     */
    LinkTransmissionStatistics();
    
    
    /**
     * default destructor, nothing to do here
     */
    virtual ~LinkTransmissionStatistics();
    
    
    /**
     * is called by finishing the simulation.
     */
    void finish();
    
    /**
     * An Ethernet frame has started its transmission (between two modules)
     *
     * It records the start time when an Ethernet frame start its transmission at
     * a given port on a given switch with a given streamID.
     * First a 0 is stored and then a 1. This is done to receive a right angle
     * in result output.
     *
     * @param switchMAC integer value of the switch MAC address to identifies the switch
     * @param portIndex integer value of a specific port in the switch.
     * @param streamID integer value of the streamID which identifies the Ethernet frame
     */
    void ethernetFrameStartedSending(int switchMAC, int portIndex, int streamID);
    
    /**
     *  An Ethernet frame has finished its transmission (between two modules)
     *
     * It records the finishing time when an Ethernet frame start its transmission at
     * a given port on a given switch with a given streamID.
     * First a 0 is stored and then a 1. This is done to receive a right angle
     * in result output.
     *
     * @param switchMAC integer value of the switch MAC address to identifies the switch
     * @param portIndex integer value of a specific port in the switch.
     * @param streamID integer value of the streamID which identifies the Ethernet frame
     */
    void ethernetFrameFinishedSending(int switchMAC, int portIndex, int streamID);

private:

    //vector object which is 3D map for storing link transmission statistics for Ethernet frame
    VectorStatistics3DMap* activationPeriod = 0;
};

#endif
