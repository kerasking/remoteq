/*
 * close.cpp
 *
 *  Created on: 2014-10-9
 *      Author: qianqians
 */
#include <pool/objpool.h>

#include "queueimpl.h"

#include "../close.h"
#include "../ipv4/windows/endpointimpl.h"
#include "../ipv4/windows/channelimpl.h"
#include "../ipv4/windows/acceptorimpl.h"

namespace Fossilizid{
namespace remoteq {

void close(HANDLE _handle){
	handle * h = (handle*)_handle;
	switch(h->_handle_type)
	{
	case handle_ipv4_tcp_acceptor_type:
		{
			ipv4::tcp::channelimpl * impl = (ipv4::tcp::channelimpl*)h;
			pool::objpool<ipv4::tcp::channelimpl>::deallocator(impl, 1);
		}
		break;

	case handle_ipv4_udp_acceptor_type:
		{
			ipv4::udp::channelimpl * impl = (ipv4::udp::channelimpl*)h;
			pool::objpool<ipv4::udp::channelimpl>::deallocator(impl, 1);
		}
		break;
	
	case handle_ipv4_tcp_channel_type:
		{
			ipv4::tcp::acceptorimlp * impl = (ipv4::tcp::acceptorimlp*)h;
			pool::objpool<ipv4::tcp::acceptorimlp>::deallocator(impl, 1);
		}
		break;
	
	case handle_ipv4_udp_channel_type:
		{
			ipv4::udp::acceptorimlp * impl = (ipv4::udp::acceptorimlp*)h;
			pool::objpool<ipv4::udp::acceptorimlp>::deallocator(impl, 1);
		}
		break;

	case handle_queue_type:
		{
			queueimpl * impl = (queueimpl*)h;
			pool::objpool<queueimpl>::deallocator(impl, 1);
		}
		break;

	case handle_ipv4_endpoint_type:
		{
			ipv4::endpointimpl * impl = (ipv4::endpointimpl*)h;
			pool::objpool<ipv4::endpointimpl>::deallocator(impl, 1);
		}
		break;

	default:
		break;
	}
}

} /* namespace remoteq */
} /* namespace Fossilizid */