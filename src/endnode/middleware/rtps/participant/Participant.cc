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

#include "../../../../endnode/middleware/rtps/participant/Participant.h"

#include "../../../../endnode/middleware/rtps/messages/RegisterEntity_m.h"
#include "../../../../endnode/middleware/rtps/messages/RTPSEthMessage_m.h"
#include "../../../../common/ValidationHandler.h"

#include "string"
#include "sstream"

Participant::~Participant()
{
}

void Participant::finish()
{
    delete writerIds;
    delete readerIds;
}


void Participant::initialize()
{
    // Get the number of readers and writers from the RTPS parent module
    nbrWriters = getParentModule()->par("numWriter");
    nbrReaders = getParentModule()->par("numReader");

    // Load the participant ID
    participantId = getParentModule()->par("participantId");

    // Initialize an ID registry for the number of readers and writers
    writerIds = new int[nbrWriters];
    readerIds = new int[nbrReaders];

    // RTPS port calculation
//    int PB = getParentModule()->par("PB");
//    int DG = getParentModule()->par("DG");
//    int PG = getParentModule()->par("PG");
//    int domainId = getParentModule()->par("domainId");
//    participantId = getParentModule()->par("participantId");
//    int d0 = getParentModule()->par("d0");
//    int d1 = getParentModule()->par("d1");
//    int d2 = getParentModule()->par("d2");
//    int d3 = getParentModule()->par("d3");
//
//    discoveryMulticastTrafficPort = PB + DG * domainId + d0;
//    discoveryUnicastTrafficPort =   PB + DG * domainId + d1 + PG * participantId;
//    userMulticastTrafficPort =      PB + DG * domainId + d2;
//    userUniCastTrafficPort =        PB + DG * domainId + d3 + PG * participantId;



}




void Participant::handleMessage(cMessage *msg)
{

    // Get the gate context of the message
    cGate* inGate = msg->getArrivalGate();
    if(inGate == nullptr){
        ValidationHandler::ThrowError("No valid gate");
        return;
    }

    int gateIndexIn = inGate->getIndex();

    // Register on new registry messages and mapping ports to ids.
    if (dynamic_cast<RegisterEntity*>(msg)!=NULL){
        RegisterEntity *reg_msg = check_and_cast<RegisterEntity*>(msg);

        // Register the Reader corresponding to the port index the message was received on.
        if(reg_msg->getReader()){
            readerIds[gateIndexIn] = reg_msg->getEntityId();
        }

        // Register the Writer corresponding to the port index the message was received on.
        if(reg_msg->getWriter()){
            writerIds[gateIndexIn] = reg_msg->getEntityId();
        }
        delete msg;
        return;
    }


    if(inGate->getName() == gate("adapterIn")->getName())
    {

        // Check for valid messages
        if(dynamic_cast<RTPSEthMessage*>(msg)==NULL)
        {
            ValidationHandler::ThrowError("Error: The participant can only dispatch messages from type RTPSEthMessage\n");
        }

        // Cast the message after a successful check
        RTPSEthMessage *rtps_msg = check_and_cast<RTPSEthMessage*>(msg);

        // Get the reader and writer IDs
        int writerId = rtps_msg->getWriterId();
        int readerId = rtps_msg->getReaderId();

        // Figure out if the receiving entity of the message is a reader or a writer
        bool sendToReader = rtps_msg->getDataFragIsSet() || rtps_msg->getDataIsSet() || rtps_msg->getHeartBeatFragIsSet() || rtps_msg->getHeartBeatFragIsSet();
        bool sendToWriter = rtps_msg->getAckNackIsSet() || rtps_msg->getNackFragIsSet();

        // Sanity Check
        if(sendToReader && sendToWriter){
            ValidationHandler::ThrowError("ERROR: RTPS message contains submessages specific for a reader and a writer.  This is not supported right now!");
        }

        if(!sendToReader && !sendToWriter){
            ValidationHandler::ThrowError("ERROR: RTPS message contains no submessages! This is not allowed!");
        }

        // If the receiver is a reader (based on the submessage evaluation)
        if(sendToReader)
        {

            // Get the corresponding readers gate index
            int gateIndexOut = -1;

            for(int i = 0; i < nbrReaders; i++)
            {
                if(readerId == readerIds[i])
                {
                    gateIndexOut = i;
                    break;
                }
            }

            // Check success
            if(gateIndexOut == -1)
            {
                std::stringstream ss;
                ss << "ERROR: No matching entity id in reader entities. Received entity id: " << readerId << "\n";
                ValidationHandler::ThrowError(ss.str().c_str());
            }

            // Forward the message and return
            send(msg, "readerOut",gateIndexOut);
            return;

        }

        // If the receiver is a writer (based on the submessage evaluation)
        if(sendToWriter)
        {
            // Get the corresponding writers gate index
            int gateIndexOut = -1;

            for(int i = 0; i < nbrWriters; i++)
            {
                if(writerId == writerIds[i])
                {
                    gateIndexOut = i;
                    break;
                }
            }

            // Check success
            if(gateIndexOut == -1)
            {
                std::stringstream ss;
                ss << "ERROR: No matching writers entity id in writer entities. Received entity id: " << readerId << "\n";
                ValidationHandler::ThrowError(ss.str().c_str());
            }

            // Forward the message and return
            send(msg, "writerOut",gateIndexOut);
            return;

        }
    }



    // Always forward outgoing messages from the writers and readers towards the shaper
    if(gateSize("writerIn")>0)
    {
        if(inGate->getName() == gate("writerIn",0)->getName())
        {
            send(msg,"shaperOut");
        }
    }

    if(gateSize("readerIn")>0)
    {
        if(inGate->getName() == gate("readerIn",0)->getName())
        {
            send(msg,"shaperOut");
        }
    }

}


