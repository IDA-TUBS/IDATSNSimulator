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

#include "E2ELatencyStatistics.h"


E2ELatencyStatistics::E2ELatencyStatistics()
{
    //nothing to do here
}


E2ELatencyStatistics::~E2ELatencyStatistics()
{
    // nothing to do here, finish takes care of everything
}


int E2ELatencyStatistics::addE2ELatency(int receiverID, int globalStreamID, int priority, omnetpp::simtime_t delay)
{
    /* collect in vector */
    if (this->vectorE2EStatistics == 0)
    {
        this->vectorE2EStatistics = new VectorStatistics2DMap("E2E-delay", "Recv", "SID", "Priority");
    }
    this->vectorE2EStatistics->addLatency(receiverID, globalStreamID, delay, priority);

    /* collect in histogram */
    if (this->histogramE2Estatistics == 0){
        this->histogramE2Estatistics = new HistogramStatistics2DMap("E2E-delay", "Recv", "SID");
    }
    this->histogramE2Estatistics->addLatency(receiverID, globalStreamID, delay);

    return 1;

}

void E2ELatencyStatistics::finish()
{

    if (this->vectorE2EStatistics)
    {
        this->vectorE2EStatistics->finish();
        delete this->vectorE2EStatistics;
        this->vectorE2EStatistics = nullptr;
    }

    if (this->histogramE2Estatistics){
        //here in finish the histogram is recored and stored to file
        this->histogramE2Estatistics->finish();
        delete this->histogramE2Estatistics;
        this->histogramE2Estatistics = nullptr;
    }

}
