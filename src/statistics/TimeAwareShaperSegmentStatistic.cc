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

#include "TimeAwareShaperSegmentStatistic.h"

TimeAwareShaperSegmentStatistic::TimeAwareShaperSegmentStatistic()
{
    // nothing to do here

}

TimeAwareShaperSegmentStatistic::~TimeAwareShaperSegmentStatistic()
{
    // nothing to do here
}

void TimeAwareShaperSegmentStatistic::criticalSendingSegmentStarts(int switchMac, int portIndex)
{
    if (this->tasPortTimeSegments == 0)
    {
        this->tasPortTimeSegments = new VectorStatistics2DMap("TAS", "SW", "port");
    }

    this->tasPortTimeSegments->addInteger(switchMac, portIndex, CRITICAL_SENDING_SEGMENT);
}


void TimeAwareShaperSegmentStatistic::criticalGuardbandSegmentStarts(int switchMac, int portIndex)
{
    if (this->tasPortTimeSegments == 0)
    {
        this->tasPortTimeSegments = new VectorStatistics2DMap("TAS", "SW", "port");
    }
    this->tasPortTimeSegments->addInteger(switchMac, portIndex, CRITICAL_GUARDBAND_SEGMENT);
}


void TimeAwareShaperSegmentStatistic::noncriticalSendingSegmentStarts(int switchMac, int portIndex)
{
    if (this->tasPortTimeSegments == 0)
    {
        this->tasPortTimeSegments = new VectorStatistics2DMap("TAS", "SW", "port");
    }
    this->tasPortTimeSegments->addInteger(switchMac, portIndex, NONCRITICAL_SENDING_SEGMENT);
}


void TimeAwareShaperSegmentStatistic::noncriticalGuardbandSegmentStarts(int switchMac, int portIndex)
{
    if (this->tasPortTimeSegments == 0)
    {
        this->tasPortTimeSegments = new VectorStatistics2DMap("TAS", "SW", "port");
    }
    this->tasPortTimeSegments->addInteger(switchMac, portIndex, NONCRITICAL_GUARDBAND_SEGMENT);
}

void TimeAwareShaperSegmentStatistic::finish()
{
    if (this->tasPortTimeSegments)
    {
        this->tasPortTimeSegments->finish();
        delete this->tasPortTimeSegments;
        this->tasPortTimeSegments = nullptr;
    }

}
