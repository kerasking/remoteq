/*
 * acceptorimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _ipv4_tcp_acceptorimlp_h
#define _ipv4_tcp_acceptorimlp_h

#include "../../../windows/handle.h"
#include "../../../typedef.h"

namespace Fossilizid{
namespace remoteq{
namespace ipv4{
namespace tcp{

struct acceptorimlp : public handle{
	acceptorimlp(QUEUE que, ENDPOINT ep);

	char * outbuf;
	SOCKET sl, sa;
	QUEUE que;
};

} /* namespace tcp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_ipv4_tcp_acceptorimlp_h

#endif //_WINDOWS
