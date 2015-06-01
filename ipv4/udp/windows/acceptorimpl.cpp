/*
 * acceptorimpl.cpp
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS

#include <exception>

#include <pool/objpool.h>

#include "acceptorimpl.h"

#include "../../../acceptor.h"

#include "../../../windows/overlapped.h"
#include "../../../windows/queueimpl.h"
#include "../../windows/endpointimpl.h"

namespace Fossilizid{
namespace remoteq {
namespace ipv4 {
namespace udp{

acceptorimlp::acceptorimlp(QUEUE _que, ENDPOINT ep){
	_handle_type = handle_ipv4_udp_acceptor_type;
	que = _que;

	s = socket(AF_INET, SOCK_DGRAM, 0);

	if (bind(s, (sockaddr*)(&((endpointimpl*)((handle*)ep))->addr), sizeof(SOCKADDR_IN)) != 0){
		throw std::exception("bind error");
	}

	int corenum = 8;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	corenum = info.dwNumberOfProcessors;

	if (CreateIoCompletionPort((HANDLE)s, ((queueimpl*)((handle*)_que))->iocp, 0, corenum) != ((queueimpl*)((handle*)_que))->iocp){
		throw std::exception("CreateIoCompletionPort error", WSAGetLastError());
	}

	memset(outbuf, 0, sizeof(outbuf));

	from = pool::objpool<endpointimpl>::allocator(1);
	new (from)endpointimpl();

	WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
	wsabuf->buf = outbuf;
	wsabuf->len = 65536;
	DWORD bytes = 0;
	DWORD flags = 0;
	overlappedex * ovlp = pool::objpool<overlappedex>::allocator(1);
	new (ovlp)overlappedex();
	ovlp->h = (handle*)this;
	ovlp->com_type = iocp_type_ipv4_udp;
	ovlp->event_type = type_recv;
	OVERLAPPED * ovp = static_cast<OVERLAPPED *>(ovlp);
	memset(ovp, 0, sizeof(OVERLAPPED));
	((endpointimpl*)from)->len = sizeof(SOCKADDR_IN);
	if (WSARecvFrom(s, wsabuf, 1, &bytes, &flags, (sockaddr*)(&((endpointimpl*)from)->addr), &((endpointimpl*)from)->len, ovp, 0) == SOCKET_ERROR){
		auto error = WSAGetLastError();
		if (error != WSA_IO_PENDING){
			throw std::exception("WSARecvFrom error", error);
		}
	}
}

ACCEPTOR acceptor(QUEUE que, ENDPOINT ep){
	acceptorimlp * impl = (acceptorimlp*)pool::objpool<acceptorimlp>::allocator(1);
	new (impl)acceptorimlp(que, ep);

	return (ACCEPTOR)((handle*)impl);
}

CHANNEL accept(ACCEPTOR ap){
	acceptorimlp * impl = (acceptorimlp*)((handle*)ap);

	CHANNEL ch = 0;
	if (impl->chque.pop(ch)){
		return ch;
	}

	return 0;
}

} /* namespace udp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_WINDOWS
