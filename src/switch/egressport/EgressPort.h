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

#ifndef CLASSES_SWITCHES_COMPONENTS_EGRESSPORT_H_
#define CLASSES_SWITCHES_COMPONENTS_EGRESSPORT_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * In this EgressPort Module nothing happend here
 */
class EgressPort : public cModule
{
public:
    /**
    *  Constructor EgressPort, nothing to do here
    */
    EgressPort();

    /**
     *  Destructor EgressPort, nothing to do here
     */
    virtual ~EgressPort();
};

Define_Module(EgressPort);

#endif /* CLASSES_SWITCHES_COMPONENTS_EGRESSPORT_H_ */
