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

} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_WINDOWS