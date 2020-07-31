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

#ifndef SRC_STATISTICS_TIMEAWARESHAPERSEGMENTSTATISTIC_H_
#define SRC_STATISTICS_TIMEAWARESHAPERSEGMENTSTATISTIC_H_

#include <omnetpp.h>
#include <map>

#include "helperModulesStatistics/VectorStatistics2DMap.h"


/**
 * Enum of TAS time segments
 */
enum tasTimeSegments
{
    CRITICAL_SENDING_SEGMENT      =  2,
    CRITICAL_GUARDBAND_SEGMENT    =  1,
    NONCRITICAL_SENDING_SEGMENT   = -2,
    NONCRITICAL_GUARDBAND_SEGMENT = -1,

};

/**
 * Time aware shaper (IEEE802.1Qbv) is a circuit-switched arbitration. To evaluate
 * the arbiter the TimeAwareShaperSegmentStatistics draws the four time segments:
 * 1. Critical sending, 2. critical guardband, 3. noncritical sending,
 * 4. noncritical guardband.
 */
class TimeAwareShaperSegmentStatistic
{
public:
    /**
     * Auto-generated constructor, nothing to do here
     */
    TimeAwareShaperSegmentStatistic();

    /**
     * Auto-generated destructor, nothing to do here
     */
    virtual ~TimeAwareShaperSegmentStatistic();

    /**
     * This function is called when the TAS critical sending segment starts. For
     * each switch and port within the switch it is set.
     *
     * @param switchMac integer value of switch MAC address to identify the switch
     * @param portIndex integer value of the port index in the switch
     */
    void criticalSendingSegmentStarts(int switchMac, int portIndex);

    /**
     * This function is called when the TAS critical guardband segment starts. For
     * each switch and port within the switch it is set.
     *
     * @param switchMac integer value of switch MAC address to identify the switch
     * @param portIndex integer value of the port index in the switch
     */
    void criticalGuardbandSegmentStarts(int switchMac, int portIndex);

    /**
     * This function is called when the TAS noncritical sending segment starts. For
     * each switch and port within the switch it is set.
     *
     * @param switchMac integer value of switch MAC address to identify the switch
     * @param portIndex integer value of the port index in the switch
     */
    void noncriticalSendingSegmentStarts(int switchMac, int portIndex);

    /**
     * This function is called when the TAS noncritical guardband segment starts. For
     * each switch and port within the switch it is set.
     *
     * @param switchMac integer value of switch MAC address to identify the switch
     * @param portIndex integer value of the port index in the switch
     */
    void noncriticalGuardbandSegmentStarts(int switchMac, int portIndex);

    /**
     * finishes off all individual collections (see individual functions),
     * frees memory and resets pointer
     */
    virtual void finish();

private:
    VectorStatistics2DMap* tasPortTimeSegments = 0;
};

#endif
