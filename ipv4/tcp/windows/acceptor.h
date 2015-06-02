/*
 * acceptor.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _ipv4_tcp_acceptor_h
#define _ipv4_tcp_acceptor_h

#include "../../../typedef.h"

namespace Fossilizid{
namespace remoteq{
namespace ipv4{
namespace tcp{
/*
 * create a ACCEPTOR
 */
ACCEPTOR acceptor(QUEUE que, ENDPOINT ep);

/*
 * accept CHANNEL from ACCEPTOR
 */
CHANNEL accept(ACCEPTOR ap);

} /* namespace tcp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_ipv4_tcp_acceptor_h