/*
 * channel.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _ipv4_tcp_channel_h
#define _ipv4_tcp_channel_h

#include <string>
#include <boost/shared_ptr.hpp>
#include "typedef.h"

#ifdef _WINDOWS
#include "../windows/channelimpl.h"
#endif

namespace Fossilizid{
namespace remoteq {
namespace ipv4{
namespace tcp{

/*
 * connect to remote endpoint
 */
CHANNEL connect(ENDPOINT ep, QUEUE que = 0);

} /* namespace tcp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_ipv4_tcp_channel_h