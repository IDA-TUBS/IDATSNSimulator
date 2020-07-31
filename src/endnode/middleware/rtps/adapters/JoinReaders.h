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

#ifndef _RTPS_ETHERNETADAPTER_H_
#define _RTPS_ETHERNETADAPTER_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * Implementation of a join.
 * All input messages are directly forwarded to the single output.
 */
class JoinReaders : public cSimpleModule
{


  public:
    /**
     * The default constructor.
     */
    JoinReaders() {}
    /**
     * The default destructor
     */
    virtual ~JoinReaders();

  protected:

    /**
     * Nothing to initialize here.
     */
    virtual void initialize() override;
    /*
     * Forward the incomning messages
     *
     * @param msg The message to forward
     */
    virtual void handleMessage(cMessage *msg) override;
    /**
     * Nothing to do after finishing.
     */
    virtual void finish() override;
};




#endif /* _RTPS_ETHERNETADAPTER_H_ */
