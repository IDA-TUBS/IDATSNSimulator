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

#include "../trafficsrcs/TraceSource.h"

#include "../../../common/GlobalMacros.h"
#include "../../../common/ValidationHandler.h"
#include "../../../messagetype/TraceGenerateMessage_m.h"

using namespace omnetpp;

TraceSource::TraceSource()
{
    // Initialize the module variables
    generateNextFrameFromTrace = nullptr;

}

TraceSource::~TraceSource()
{
    // Clean the self messages
    cancelAndDelete(generateNextFrameFromTrace);
}


void TraceSource::initialize()
{

    // Load the global stream ID of the generated stream
    globalStreamId = getParentModule()->par("globalStreamId");

    // Load startup parameters
    generatorEnable = getParentModule()->par("isEnabled").boolValue();
    simtime_t offset = getParentModule()->par("offsetDuration");
    bool usingOffset = getParentModule()->par("enableOffset").boolValue();

    // Initialize the self-messages
    generateNextFrameFromTrace = new cMessage("generateNextFrameFromTrace", 0);

    //Load data for finding the trace files
    const char* fileName = getParentModule()->par("fileName").stringValue();

    _burst_src_verbose_
    {
        EV << "[Trace Source] global stream ID: \t" << globalStreamId << "\n";
        EV << "[Trace Source] is enabled: \t" << generatorEnable << "\n";
        EV << "[Trace Source] trace file: \t" << fileName << "\n";
    }

    // Start the generator if enabled
    if (generatorEnable)
    {
        FILE * traceFile;
        traceIndex=0;
        packetArrivalDelayArraySize = 0;

        float tmp;
        maxTraceLength = this->getParentModule()->par("maxTraceLength");
        packetArrivalDelayArray = new double[maxTraceLength];
        packetSizeArray = new int[maxTraceLength];

        for(int i=0; i < maxTraceLength; i++) { //clear array
            packetArrivalDelayArray[i] = 0;
            packetSizeArray[i] = 0;
        }


        // File pointer
        std::fstream fin;

        // Open an existing file
        fin.open(fileName, std::ios::in);
        // as String Vector
        std::vector<std::string> row;
        std::string line, word;
        while (std::getline(fin, line)) // read whole line into line
        {
            row.clear();
            std::istringstream iss(line); // string stream
            while (std::getline(iss, word, '|')) {

                // add all the column data
                // of a row to a vector
                row.push_back(word);
            }

            float scaleFactor = this->getParentModule()->par("scaleFactor").doubleValue();
            packetArrivalDelayArray[packetArrivalDelayArraySize]= scaleFactor*stof(row[1]); ; // trace file input is in ns
            packetSizeArray[packetArrivalDelayArraySize]=stoi(row[4]);
            packetArrivalDelayArraySize++;
        }



        // Initialize generation message
        if (usingOffset)
        {
            // If enabled start the burst generator with an offset
            scheduleAt(simTime() + offset, generateNextFrameFromTrace);
        }
        else
        {
            // Start burst generator without an offset
            scheduleAt(simTime(), generateNextFrameFromTrace);
        }
    }
}



void TraceSource::handleMessage(cMessage *msg)
{

    // Check for incoming generator messages
    if (msg->isSelfMessage())
    {
        // Check for next burst element to send
        if (msg == generateNextFrameFromTrace)
        {
            // A packet within a burst is being generated now.
            // Send packet
            // Send out a message
            TraceGenerateMessage *genMsg = new TraceGenerateMessage();
            genMsg->setPacketSize(packetSizeArray[traceIndex]);
            EV << "[Trace Source] packet size: \t" << packetSizeArray[traceIndex] << "\n";
            send(genMsg, "messageGenerationOut");
            //schedule next

            if(traceIndex < packetArrivalDelayArraySize-1)
            {
                EV << "Trace index : " << traceIndex << "\n";
                scheduleAt(simTime() + packetArrivalDelayArray[traceIndex+1] - packetArrivalDelayArray[traceIndex], generateNextFrameFromTrace);
                EV << "Next packet in : " << packetArrivalDelayArray[traceIndex+1] - packetArrivalDelayArray[traceIndex] << "\n";
                traceIndex++;
            }
          }
        // There should not be any other message types
        else
        {
            ValidationHandler::ThrowError("ERROR: Receiving unexpected message!");
        }
    }
    // There should only be self-messages scheduled in this module
    else
    {
        ValidationHandler::ThrowError("ERROR: Receiving unexpected message!");
    }
}














