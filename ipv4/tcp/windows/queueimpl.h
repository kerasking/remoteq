/*
 * queueimpl.h
 *
 *  Created on: 2015-5-23
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _ipv4_tcp_queueimpl_h
#define _ipv4_tcp_queueimpl_h

#include "../ipv4/windows/acceptorimpl.h"
#include "../ipv4/windows/channelimpl.h"
#include "../ipv4/windows/endpointimpl.h"

namespace Fossilizid{
namespace remoteq {
namespace ipv4{
namespace tcp {

void dispense(uint16_t type, queueimpl * impl, EVENT & ev, DWORD bytes, handle * h){
	if (type == type_accept){
		ev.type = event_type_ipv4_tcp_accept;
		ev.handle.acp = (ACCEPTOR)((acceptorimlp*)h);
	} else if (type == type_recv){
		channelimpl* ch = ((channelimpl*)h);
		if (bytes == 0){
			EVENT ev;
			ev.handle.ch = (CHANNEL)((channelimpl*)h);
			ev.type = event_type_ipv4_tcp_disconnect;
			impl->evque.push(ev);
		} else{
			ev.type = event_type_ipv4_tcp_recv;
			ev.handle.ch = (CHANNEL)((channelimpl*)h);
			((channelimpl*)h)->windex += bytes;
		}
	}
}

} /* namespace tcp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_ipv4_tcp_queueimpl_h

#endif //_WINDOWS