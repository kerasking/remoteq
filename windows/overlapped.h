/*
 * overlapped.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _overlapped_h
#define _overlapped_h

#include <boost/shared_ptr.hpp>
#include <string>

#include "handle.h"

namespace Fossilizid{
namespace remoteq {

enum iocp_type_height{
	iocp_type_ipv4_tcp,
	iocp_type_ipv4_udp,
};

enum iocp_type_lower{
	type_accept,
	type_send,
	type_recv,
};

struct overlappedex : public OVERLAPPED{
	handle * h;
	uint16_t com_type;
	uint16_t event_type;

	struct {
		//only used by send
		boost::shared_ptr<std::string> buf;
	} sendbuf;
};

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_overlapped_h

#endif //_WINDOWS