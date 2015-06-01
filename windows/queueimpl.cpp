/*
 * queueimpl.cpp
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#include <pool/objpool.h>

#include "../endpoint.h"

#include "queueimpl.h"
#include "overlapped.h"

#include "../ipv4/tcp/windows/queueimpl.h"
#include "../ipv4/udp/windows/queueimpl.h"

namespace Fossilizid{
namespace remoteq {

QUEUE queue(){
	queueimpl * impl = new queueimpl;

	int corenum = 8;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	corenum = info.dwNumberOfProcessors;

	impl->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, corenum);

	return (QUEUE)((handle*)impl);
}

EVENT queue(QUEUE que){
	queueimpl * impl = (queueimpl *)((handle*)que);

	EVENT ev; 
	ev.type = event_type_none;
	ev.handle.acp = 0;
	
	do{
		if (impl->evque.pop(ev)){
			break;
		}

		DWORD bytes = 0;
		ULONG_PTR ptr = 0;
		LPOVERLAPPED ovp = 0;
		if (GetQueuedCompletionStatus(impl->iocp, &bytes, &ptr, &ovp, 0)){
			overlappedex * ovlp = static_cast<overlappedex *>(ovp);
			
			if (ovlp->com_type == iocp_type_ipv4_tcp){
				ipv4::tcp::dispense(ovlp->event_type, impl, ev, bytes, ovlp->h);
			} else if (ovlp->com_type == iocp_type_ipv4_udp){
				ipv4::udp::dispense(ovlp->event_type, impl, ev, bytes, ovlp->h);
			}

			pool::objpool<overlappedex>::deallocator(ovlp, 1);
		}
	} while (0);

	return ev;
}

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_WINDOWS