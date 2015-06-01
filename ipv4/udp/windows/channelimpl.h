/*
 * channelimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _udp_channelimpl_h
#define _udp_channelimpl_h

#include <exception>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>

#include <pool/objpool.h>

#include "../../../typedef.h"

#include "../../../windows/handle.h"
#include "../../../windows/overlapped.h"
#include "../../../windows/queueimpl.h"
#include "../../windows/endpointimpl.h"
#include "../../windows/acceptorimpl.h"

namespace Fossilizid{
namespace remoteq{
namespace ipv4{
namespace udp{

struct channelimpl : public handle{
	channelimpl(QUEUE _que, SOCKET s_, ENDPOINT _ep){
		_handle_type = handle_ipv4_udp_channel_type;
		s = s_;
		que = _que;
		ep = _ep;

		from = pool::objpool<endpointimpl>::allocator(1);
		new (from)endpointimpl();

		rindex = 0;
		windex = 0;
		buflen = 65536;
		buf = (char*)pool::mempool::allocator(buflen);
	}

	QUEUE que;
	ENDPOINT ep;
	ENDPOINT from;

	ACCEPTOR acp;

	char * buf;
	int buflen, rindex, windex;
	SOCKET s;
};

bool push(CHANNEL ch, boost::shared_ptr<std::string> buf){
	WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
	wsabuf->buf = const_cast<char*>(buf->c_str());
	wsabuf->len = buf->size();
	DWORD bytes = 0;
	overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
	new (ovp)overlappedex();
	ovp->h = (handle*)(channelimpl*)((handle*)ch);
	ovp->com_type = iocp_type_ipv4_udp;
	ovp->event_type = type_send;
	ovp->sendbuf.buf = buf;
	OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovp);
	memset(ovp_, 0, sizeof(OVERLAPPED));
	((endpointimpl*)(((channelimpl*)((handle*)ch))->ep))->len = sizeof(SOCKADDR_IN);
	if (WSASendTo(((channelimpl*)((handle*)ch))->s, wsabuf, 1, &bytes, 0, (sockaddr*)(&(((endpointimpl*)(((channelimpl*)((handle*)ch))->ep))->addr)), ((endpointimpl*)(((channelimpl*)((handle*)ch))->ep))->len, ovp_, 0) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING){
			EVENT ev;
			ev.handle.ch = ch;
			ev.type = event_type_ipv4_udp_disconnect;
			((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

			return false;
		}
	}

	return true;
}

boost::shared_ptr<std::string> pop(CHANNEL ch){
	channelimpl * implch = (channelimpl*)((handle*)ch);
	if (((channelimpl*)((handle*)ch))->que == 0){
		while (1){
			char * buf = implch->buf + implch->windex;
			int buflen = implch->buflen - implch->windex;
			((endpointimpl*)implch->from)->len = sizeof(SOCKADDR_IN);
			int len = recvfrom(((channelimpl*)((handle*)implch))->s, buf, buflen, 0, (sockaddr*)(&((endpointimpl*)implch->from)->addr), &((endpointimpl*)implch->from)->len);

			if (len < 0){
				EVENT ev;
				ev.handle.ch = ch;
				ev.type = event_type_ipv4_udp_disconnect;
				((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

				break;
			}

			if (((endpointimpl*)implch->from)->addr.sin_addr.S_un.S_addr != ((endpointimpl*)implch->ep)->addr.sin_addr.S_un.S_addr ||
				((endpointimpl*)implch->from)->addr.sin_port != ((endpointimpl*)implch->ep)->addr.sin_port){
				continue;
			}

			if (len > 0){
				implch->windex += len;
			}

			if (len > 0 && len < buflen){
				break;
			}

			if (len == 0){
				EVENT ev;
				ev.handle.ch = ch;
				ev.type = event_type_ipv4_udp_disconnect;
				((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

				break;
			}

			buflen = implch->buflen;
			implch->buflen *= 2;
			char * tmp = implch->buf;
			implch->buf = (char*)pool::mempool::allocator(implch->buflen);
			memcpy(implch->buf, tmp, buflen);
			pool::mempool::deallocator(tmp, buflen);
		}
	}

	if (implch->windex == implch->rindex){
		return 0;
	}

	boost::shared_ptr<std::string> ret = 0;
	if (implch->windex > 0){
		ret = boost::make_shared<std::string>(implch->buf + implch->rindex, implch->windex - implch->rindex);

		implch->rindex = 0;
		implch->windex = 0;
	}

	if (((channelimpl*)((handle*)ch))->que != 0){
		acceptorimlp * acp = (acceptorimlp *)(handle*)implch->acp;

		WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
		wsabuf->buf = acp->outbuf;
		wsabuf->len = 65536;
		DWORD bytes = 0;
		DWORD flags = 0;
		overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
		new (ovp)overlappedex();
		ovp->h = (handle*)(acceptorimlp*)((handle*)acp);
		ovp->com_type = iocp_type_ipv4_udp;
		ovp->event_type = type_recv;
		OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovp);
		memset(ovp_, 0, sizeof(OVERLAPPED));
		memset(&((endpointimpl*)acp->from)->addr, 0, sizeof(SOCKADDR_IN));
		((endpointimpl*)implch->from)->len = sizeof(SOCKADDR_IN);
		if (WSARecvFrom(acp->s, wsabuf, 1, &bytes, &flags, (sockaddr*)(&((endpointimpl*)acp->from)->addr), &((endpointimpl*)acp->from)->len, ovp_, 0) == SOCKET_ERROR){
			if (WSAGetLastError() != WSA_IO_PENDING){
				EVENT ev;
				ev.handle.ch = ch;
				ev.type = event_type_ipv4_udp_disconnect;
				((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);
			}
		}
	}

	return ret;
}

} /* namespace udp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_udp_channelimpl_h

#endif //_WINDOWS