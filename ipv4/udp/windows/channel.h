/*
 * channel.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _ipv4_udp_channel_h
#define _ipv4_udp_channel_h

#include <string>
#include <boost/shared_ptr.hpp>
#include "typedef.h"

#ifdef _WINDOWS
#include "../windows/channelimpl.h"
#endif

namespace Fossilizid{
namespace remoteq {
namespace ipv4{
namespace udp{

/*
 * create a channel to remote endpoint
 */
CHANNEL channel(ENDPOINT ep, QUEUE que = 0);

} /* namespace udp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_channel_h