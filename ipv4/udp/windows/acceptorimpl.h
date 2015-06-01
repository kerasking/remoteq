/*
 * acceptorimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _fast_acceptorimlp_h
#define _fast_acceptorimlp_h

#include <boost/unordered_map.hpp>

#include <container/msque.h>

#include "../../../windows/handle.h"
#include "../../../typedef.h"

namespace Fossilizid{
namespace remoteq{
namespace ipv4{
namespace udp{

struct acceptorimlp : public handle{
	acceptorimlp(QUEUE que, ENDPOINT ep);

	char outbuf[65536];

	SOCKET s;
	QUEUE que;
	ENDPOINT from;

	container::msque<CHANNEL> chque;

	boost::unordered_map<uint64_t, CHANNEL> channels;

};

} /* namespace udp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_fast_acceptorimlp_h

#endif //_WINDOWS
