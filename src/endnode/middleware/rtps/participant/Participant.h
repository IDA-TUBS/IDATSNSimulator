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

#ifndef SRC_ENDNODE_MIDDLEWARE_RTPS_PARTICIPANT_PARTICIPANT_H_
#define SRC_ENDNODE_MIDDLEWARE_RTPS_PARTICIPANT_PARTICIPANT_H_


#include <omnetpp.h>
using namespace omnetpp;

/**
 * Dispatches the messages and pass them either to the correct reader/writer for receiving or to the shaper for sending
 */
class Participant : public cSimpleModule
{
  private:

    /// The globally unique ID of the participant
    int participantId;

    /// Store the number of reader and writer locally
    int nbrWriters;
    int nbrReaders;

    /// Registry for reader IDs to  gate-index mappings
    int* writerIds;
    /// Registry for writer IDs to  gate-index mappings
    int* readerIds;

    // Port values for the discovery
//    int discoveryMulticastTrafficPort;
//    int discoveryUnicastTrafficPort;
//    int userMulticastTrafficPort;
//    int userUniCastTrafficPort;


  public:
    /**
     * Default constructor
     */
    Participant() {}

    /**
     * Default destructor
     */
    virtual ~Participant();

  protected:

    /**
     * Initialization to load the module parameters
     */
    virtual void initialize() override;

    /**
     * Handles and dispatches (forwards) incoming messages
     *
     * @param msg The incoming msg to forward, based on its origin and content
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Deletes all stored context
     */
    virtual void finish() override;

};

Define_Module(Participant);

#endif
