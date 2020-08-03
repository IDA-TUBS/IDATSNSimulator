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

#ifndef SRC_COMMON_GLOBALMACROS_H_
#define SRC_COMMON_GLOBALMACROS_H_

#include <omnetpp.h>
#include <unordered_set>

using namespace omnetpp;

/*-- TYPEDEF definitions --*/
typedef std::vector<std::vector<std::string> > csvVector;
struct Frer_ID{
    int port_ids[2];
};

/*-- MACRO/FUNCTION definitions  --*/

#define _min_(x,y) (y<x?y:x)
#define _max_(x,y) (x>y?x:y)

/*--------------------------*/


/*-- COLOUR definitions for messages  --*/
#define _msg_red_       0
#define _msg_green_     1
#define _msg_blue_      2
#define _msg_white_     3
#define _msg_yellow_    4
#define _msg_cyan_      5
#define _msg_magenta_   6
#define _msg_black_     7

/*-- Lookup delays --*/
//////////////////////////////
#define _CAN2CAN_LOOKUP_    0
#define _CAN2ETH_LOOKUP_    0
#define _ETH2CAN_LOOKUP_    0
#define _ETH2ETH_LOOKUP_    0
/*--------------------------*/


/*-- METHOD definitions --*/
////////////////////////////
/***
 * Truncates the input with the minimum and maximum.
 * The maximum is disabled with max=0.
 */
template<typename T> inline T truncate_min_max(T input, T min, T max)
{
    T result = input;
    if (input < min)
    {
        result = min;
        EV << "-I- " << " Input " << input << " truncated to the minimum " << min << " .\n";
    }
    else if (max > 0 && input > max)
    {
        result = max;
        EV << "-I- " << " Input " << input << " truncated to the maximum " << min << " .\n";
    }
    return result;
}

/**
 * s1 % s2; what is the reminder of dividing s1 by s2
 */
inline omnetpp::simtime_t simtime_modulo(omnetpp::simtime_t s1, omnetpp::simtime_t s2)
{
    omnetpp::simtime_t temp = s1;
    while (temp >= s2)
    {
        temp -= s2;
    }
    return temp;
}


struct lostFrameCounter{
    int newestFrameID;
    std::unordered_set <int> lostFrames;

};

#endif
