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

#include "../../../../endnode/middleware/rtps/delayunit/DelayUnit.h"

#include "../../../../common/GlobalMacros.h"

Define_Module(DelayUnit);

DelayUnit::~DelayUnit()
{
}

void DelayUnit::initialize()
{
}

void DelayUnit::handleMessage(cMessage *msg)
{
    // Calculate the probabilistic duration
    simtime_t duration = par("duration");

    // Bound the duration towards the minimum and maximum
    duration = truncate_min_max<simtime_t>(duration, par("durationMin"),  par("durationMax"));

    // Send delayed
    sendDelayed(msg, duration, "out");

}

