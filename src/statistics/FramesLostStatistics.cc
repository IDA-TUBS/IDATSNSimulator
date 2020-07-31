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

#include "../statistics/FramesLostStatistics.h"

FramesLostStatistics::FramesLostStatistics()
{
    //nothing to do here
}

FramesLostStatistics::~FramesLostStatistics()
{
    //nothing to do here
}

void FramesLostStatistics::addLostFrame(int receiverID, int globalStreamID, int counter)
{

    /* collect in vector */
    if (this->vectorLostFramesPerStream == 0)
    {
        this->vectorLostFramesPerStream = new VectorStatistics2DMap("Lost-Frames-Cntr", "Recv", "SID");
    }
    this->vectorLostFramesPerStream->addInteger(receiverID, globalStreamID, counter);

//    /* collect in histogram */
//    if (this->histogramLostFramesPerStream == 0){
//        this->histogramLostFramesPerStream = new HistogramStatistics2DMap("Lost-Frames-Hist", "Recv", "SID");
//    }
//    this->histogramLostFramesPerStream->addIntegerResult(receiverID, globalStreamID, counter);

}

void FramesLostStatistics::addLostFrameTrace(int receiverID, int globalStreamID, int counter)
{

    /* collect in vector */
    if (this->vectorLostFramesTracePerStream  == 0)
    {
        this->vectorLostFramesTracePerStream  = new VectorStatistics2DMap("Lost-Frames-Trace", "Recv", "SID");
    }
    this->vectorLostFramesTracePerStream ->addInteger(receiverID, globalStreamID, counter);
//
//    /* collect in histogram */
//    if (this->histogramLostFramesPerStream == 0){
//        this->histogramLostFramesPerStream = new HistogramStatistics2DMap("Lost-Frames-Hist", "Recv", "SID");
//    }
//    this->histogramLostFramesPerStream->addIntegerResult(receiverID, globalStreamID, counter);

}


void FramesLostStatistics::finish()
{
    if (this->vectorLostFramesPerStream)
   {
       this->vectorLostFramesPerStream->finish();
       delete this->vectorLostFramesPerStream;
       this->vectorLostFramesPerStream = nullptr;
   }

    if (this->vectorLostFramesTracePerStream)
   {
       this->vectorLostFramesTracePerStream->finish();
       delete this->vectorLostFramesTracePerStream;
       this->vectorLostFramesTracePerStream = nullptr;
   }

   if (this->histogramLostFramesPerStream){
       //here in finish the histogram is recored and stored to file
       this->histogramLostFramesPerStream->finish();
       delete this->histogramLostFramesPerStream;
       this->histogramLostFramesPerStream = nullptr;
   }
}
