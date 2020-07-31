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

#include "ddsSampleLatencyStatistics.h"

#include "../common/ValidationHandler.h"

using omnetpp::simtime_t;
using std::string;
using std::stringstream;
using std::vector;


SampleLatencyStatistics::SampleLatencyStatistics()
{
    // Nothing to be done here.
}

/**
 * Destructor.
 */
SampleLatencyStatistics::~SampleLatencyStatistics()
{
    // Nothing to be done here.
}

void SampleLatencyStatistics::report(const omnetpp::simtime_t& latency, int participantId, int readerId, int globalStreamId){

    sampleLatencyStatisticsVector->addLatency(participantId, globalStreamId, readerId, latency);
    sampleLatencyStatisticsHistogram->addLatency(participantId, globalStreamId, readerId, latency);
}


void SampleLatencyStatistics::addMessage(int participantId, int readerId, int globalStreamId, simtime_t delay)
{

    // Initialize stats on demand
    if (this->sampleLatencyStatisticsVector == nullptr)
    {
        this->sampleLatencyStatisticsVector = new VectorStatistics3DMap("E2E-decomposed-delay", "Recv", "SID", "Domain");
    }
    if (this->sampleLatencyStatisticsHistogram == nullptr)
    {
        this->sampleLatencyStatisticsHistogram = new HistogramStatistics3DMap("E2E-decomposed-delay", "Recv", "SID", "Domain");
    }

    report(delay, participantId, readerId, globalStreamId);

    return;
}


void SampleLatencyStatistics::finish()
{

    if (this->sampleLatencyStatisticsVector)
    {
        this->sampleLatencyStatisticsVector->finish();
        delete this->sampleLatencyStatisticsVector;
        this->sampleLatencyStatisticsVector = nullptr;
    }

    if (this->sampleLatencyStatisticsHistogram)
    {
        //here in finish the histogram is recored and stored to file
        this->sampleLatencyStatisticsHistogram->finish();
        delete this->sampleLatencyStatisticsHistogram;
        this->sampleLatencyStatisticsHistogram = nullptr;
    }

}
