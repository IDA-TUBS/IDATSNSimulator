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

#include "StatisticManager.h"

StatisticManager::StatisticManager()
{
    //create collections on demand
}

StatisticManager::~StatisticManager()
{
    //everything should have been taken care of in finish
}

E2ELatencyStatistics* StatisticManager::getDelayStatistics()
{
    this->hasFinishedAll = false;
    if (this->pathDelayStats == 0)
    {
        this->pathDelayStats = new E2ELatencyStatistics();
    }
    return this->pathDelayStats;
}


FramesLostStatistics* StatisticManager::getFramesLostStatistics()
{
    this->hasFinishedAll = false;
    if (this->framesLostStats == 0)
    {
        this->framesLostStats = new FramesLostStatistics();

    }
    return this->framesLostStats;
}

SampleLatencyStatistics* StatisticManager::getSampleLatencyStatistics()
{
    this->hasFinishedAll = false;
    if(this->sampleLatencyStats == 0)
    {
        this->sampleLatencyStats = new SampleLatencyStatistics();
    }
    return this->sampleLatencyStats;
}

LinkTransmissionStatistics* StatisticManager::getLinkTransmissionStatistics()
{
    this->hasFinishedAll = false;
    if (this->linkTransmission == 0)
    {
        this->linkTransmission = new LinkTransmissionStatistics();
    }
    return this->linkTransmission;
}

TimeAwareShaperSegmentStatistic* StatisticManager::getTimeAwareShaperStatistics()
{
    this->hasFinishedAll = false;
    if (this->tsnTASStatistics == 0)
    {
        this->tsnTASStatistics = new TimeAwareShaperSegmentStatistic();
    }
    return this->tsnTASStatistics;
}

void StatisticManager::finishAll()
{
    //only run finish, if necessary
    if (!this->hasFinishedAll)
    {
        this->hasFinishedAll = true;

        if (this->pathDelayStats)
        {
            this->pathDelayStats->finish();
            delete (this->pathDelayStats);
            this->pathDelayStats = nullptr;
        }

        if (this->framesLostStats)
        {
            this->framesLostStats->finish();
            delete (this->framesLostStats);
            this->framesLostStats = nullptr;
        }

        if (this->sampleLatencyStats)
        {
            this->sampleLatencyStats->finish();
            delete (this->sampleLatencyStats);
            this->sampleLatencyStats = nullptr;
        }

        if (this->linkTransmission)
        {
            this->linkTransmission->finish();
            delete (this->linkTransmission);
            this->linkTransmission = nullptr;
        }

        if (this->tsnTASStatistics)
        {
            this->tsnTASStatistics->finish();
            delete (this->tsnTASStatistics);
            this->tsnTASStatistics = nullptr;
        }

        EV << "called finishALL() in StatisticsCollector\n";
    }

}
