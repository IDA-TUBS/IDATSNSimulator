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

#ifndef SWITCH_EGRESSPORT_CLASSIFIERPRIORITY_H_
#define SWITCH_EGRESSPORT_CLASSIFIERPRIORITY_H_

#include <omnetpp.h>;
#include "../../messagetype/EthernetFrame_m.h"


using namespace omnetpp;

/**
 * The Classifier classify Frames based on the Ethernet priority and forward them in one of the priority asserted channels
 */
class ClassifierPriority : public cSimpleModule
{
public:

    /**
     * Constructor of the class, nothing to do
     */
    ClassifierPriority();

    /**
     * Destructor of the class, nothing to do
     */
    virtual ~ClassifierPriority();

    /**
     * Initialize the global Variables
     */
    void initialize();

    /**
     * Process incomming Ethernet frames and send them based on the priority out
     */
    void handleMessage(cMessage* message);

private:
    cModule* parentModul;
    int numberPriorities;
    int framePriority;

};

Define_Module(ClassifierPriority);

#endif /* SWITCH_EGRESSPORT_CLASSIFIERPRIORITY_H_ */
