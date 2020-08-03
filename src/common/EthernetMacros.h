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


#ifndef SRC_COMMON_ETHERNETMACROS_H_
#define SRC_COMMON_ETHERNETMACROS_H_

#include <math.h>
#include <stdlib.h>
#include "GlobalMacros.h"

/*-- CONSTANT definitions --*/
//////////////////////////////
#define _MIN_ETH_PAYLOAD_                           (42)
#define _MIN_ETH_PAYLOAD_UNTAGGED                   (46)
#define _MAX_ETH_PAYLOAD_                           (1500)
#define _PAYLOAD_INTERMEDIATE_PREEMPTION_           (60)
#define _FRAME_OVERHEAD_                            (6 + 6 + 4 + 2 + 4)                     //MAC dest(6)| MAC src(6)| TAG (4)|ETHT (2) | CRC (4) //SUM=22
#define _PACKET_OVERHEAD_                           (7 + 1)                                 //PREAM (7) | SoF Del (1)//SUM= 8
#define _PACKET_OVERHEAD_FIRST_PREEMPTION_          (7 + 1 + 6 + 6 + 4 + 2)                 //PREAM (7) | SMD-Sx (1) | DA (6) | SA (6) | Q-Tag (4) | ET (2)
#define _PACKET_OVERHEAD_INTERMEDIATE_PREEMPTION_   (6 + 1 + 1)                             //PREAM (6) | SMD-Cx (1) | FCnt (1)
#define _CRC_                                       (4)
#define _INTERPACKET_GAP_                           (12)
#define _CRC_INTERPACKET_GAP_                       (_INTERPACKET_GAP_ + _CRC_)
#define _MIN_ETH_FRAME_SIZE_                        (_MIN_ETH_PAYLOAD_ + _FRAME_OVERHEAD_)
#define _MIN_ETH_PACKET_SIZE_                       (_MIN_ETH_FRAME_SIZE_ + _PACKET_OVERHEAD_ + _INTERPACKET_GAP_)
#define _MIN_ETH_PACKET_SIZE_PREEMPTION_            (144)
/*--------------------------*/

//ETH specific
#define _eth_check_for_padding_(payload)        (payload>=_MIN_ETH_PAYLOAD_?payload:_MIN_ETH_PAYLOAD_)
#define _eth_check_for_max_payload_(payload)    (payload<=_MAX_ETH_PAYLOAD_?payload:_MAX_ETH_PAYLOAD_)
#define _eth_framesize_from_payload_(payload)   (payload+ _FRAME_OVERHEAD_)
#define _eth_packetsize_from_payload_(payload)  (payload + _FRAME_OVERHEAD_ + _PACKET_OVERHEAD_ + _INTERPACKET_GAP_)

#define _std_eth_colour_ (_msg_red_)
/*--------------------------*/

#endif
