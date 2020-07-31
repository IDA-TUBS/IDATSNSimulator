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

#ifndef RTPSMACROS_H_
#define RTPSMACROS_H_

#include <math.h>
#include <stdlib.h>
#include "GlobalMacros.h"
#include "EthernetMacros.h"

/// CONSTANT definitions
/// Minimum and maximum RTPS payload
#define _MIN_RTPS_PAYLOAD_                          (0)
#define _MAX_RTPS_PAYLOAD_                          (4294967296)
/// Sizes UDP IP Header
#define _UDP_HEADER_SIZE_                           (8)
#define _IP_HEADER_SIZE_                            (20)
/// Size of RTPS header
#define _RTPS_HEADER_SIZE_                          (20)
/// Sizes of relevant RTPS submessages (static part)
#define _RTPS_INFO_DST_SIZE_                        (16)
#define _RTPS_INFO_TS_SIZE_                         (12)
#define _RTPS_HEARTBEATFRAG_SIZE                    (28)
#define _RTPS_DATAFRAG_STATIC_SIZE_                 (36)
#define _RTPS_NACKFRAG_STATIC_SIZE_                 (32)
/// Maximum RTPS sizes
#define _RTPS_MAX_MSG_SIZE_                         (_MAX_ETH_PAYLOAD_ - _IP_HEADER_SIZE_ - _UDP_HEADER_SIZE_)
#define _RTPS_MAX_FRAG_SIZE_                        (_RTPS_MAX_MSG_SIZE_ - _RTPS_HEADER_SIZE_ - _RTPS_INFO_DST_SIZE_ - _RTPS_INFO_TS_SIZE_ - _RTPS_DATAFRAG_STATIC_SIZE_)
/*--------------------------*/

/// RTPS specific methods
#define _rtps_check_for_max_payload_(payload)    (payload<=_MAX_RTPS_PAYLOAD_?payload:_MAX_RTPS_PAYLOAD_)
#define _rtps_add_udp_ip_overhead_(rtps_msg_size) (rtps_msg_size + _IP_HEADER_SIZE_ + _UDP_HEADER_SIZE_)
/*--------------------------*/

/// RTPS entity kinds
enum rtpsEntityKind
{
    unknown = 0,
    participant = 1,
    writerWithKey = 2,
    writerNoKey = 3,
    readerNoKey = 4 ,
    readerWithKey = 7,
};


#endif /* RTPSMACROS_H_ */
