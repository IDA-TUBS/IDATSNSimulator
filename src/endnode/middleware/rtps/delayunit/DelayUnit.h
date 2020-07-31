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

#ifndef ENDNODE_MIDDLEWARE_RTPS_DELAYUNIT_DELAYUNIT_H_
#define ENDNODE_MIDDLEWARE_RTPS_DELAYUNIT_DELAYUNIT_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * Adds artificial delay to the delivery of the sample to the writer to model runtime overoverhead
 */
class DelayUnit : public cSimpleModule
{

  public:

    /**
     * Default constructor
     */
    DelayUnit() {}
    /**
     * Default destructor
     */
    virtual ~DelayUnit();

  protected:

    /**
     * Nothing to initialize
     */
    virtual void initialize() override;

    /**
     * Delays and forwards incoming messages
     *
     * @param msg The message to delay and forward
     */
    virtual void handleMessage(cMessage *msg) override;
};


#endif /* ENDNODE_MIDDLEWARE_RTPS_DELAYUNIT_DELAYUNIT_H_ */
