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

#include "ClassifierPriority.h"

ClassifierPriority::ClassifierPriority()
{

}

ClassifierPriority::~ClassifierPriority()
{

}

/**
 *
 */
void ClassifierPriority::initialize()
{
    this->parentModul = getParentModule();
    this->numberPriorities = (int) this->par("supportedPriorities");
}

/**
 * sort frames based on priority to class
 */
void ClassifierPriority::handleMessage(cMessage* message)
{

    EthernetFrame* ethernetFrame = check_and_cast<EthernetFrame*> (message);
    if (ethernetFrame == nullptr)
    {
        EV << "[ClassifierPriority] ERROR: Could not cast to Ethernet Frame in Ethernet Classifier!" << "\n";
        throw cRuntimeError("[ClassifierPriority] ERROR: Could not cast to Ethernet Frame in Ethernet Classifier!");
    }
    // send out arrived frames
    this->framePriority = ethernetFrame->getPriority();

    EV << "[classifierPriority] send frame out over port: out " << this->framePriority << "\n";

    send(message, "out", this->framePriority);

}
