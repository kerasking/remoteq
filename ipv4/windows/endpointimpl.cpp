/*
 * endpointimpl.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#include <pool/objpool.h>

#include "../../endpoint.h"

#include "endpointimpl.h"
#include "channelimpl.h"

namespace Fossilizid{
namespace remoteq {
namespace ipv4{

ENDPOINT endpoint(char * ip, short port){
	endpointimpl * impl = pool::objpool<endpointimpl>::allocator(1);

	memset(&impl->addr, 0, sizeof(impl->addr));
	impl->addr.sin_family = AF_INET;
	impl->addr.sin_addr.S_un.S_addr = inet_addr(ip);
	impl->addr.sin_port = port;
	impl->_handle_type = handle_ipv4_endpoint_type;

	return (ENDPOINT)((handle*)impl);
}

ENDPOINT remotep(CHANNEL ch){
	endpointimpl * eimpl = pool::objpool<endpointimpl>::allocator(1);
	memset(&eimpl->addr, 0, sizeof(eimpl->addr));
	int len = sizeof(eimpl->addr);
	
	handle * h = (handle*)ch;
	if (h->_handle_type == handle_ipv4_tcp_channel_type){
		ipv4::tcp::channelimpl * impl = (ipv4::tcp::channelimpl *)h;
		if (getpeername(impl->s, (sockaddr*)&eimpl->addr, &len) == SOCKET_ERROR){
			//throw std::exception("");
		}
	} else if (h->_handle_type == handle_ipv4_tcp_channel_type){
		ipv4::udp::channelimpl * impl = (ipv4::udp::channelimpl *)h;
		if (getpeername(impl->s, (sockaddr*)&eimpl->addr, &len) == SOCKET_ERROR){
			//throw std::exception("");
		}
	}

	return (ENDPOINT)((handle*)eimpl);
}

std::string ip(ENDPOINT ep){
	endpointimpl * eimpl = (endpointimpl *)((handle*)ep);

	return inet_ntoa(eimpl->addr.sin_addr);
}

short port(ENDPOINT ep){
	endpointimpl * eimpl = (endpointimpl *)((handle*)ep);

	return eimpl->addr.sin_port;
}

} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_WINDOWS