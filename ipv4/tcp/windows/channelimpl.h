/*
 * channelimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _tcp_channelimpl_h
#define _tcp_channelimpl_h

#include <exception>
#include <sstream>

#include <boost/function.hpp>
#include <boost/make_shared.hpp>

#include <container/msque.h>
#include <pool/mempool.h>

#include "../../../typedef.h"
#include "../../../binarystream.h"

#include "../../../windows/handle.h"
#include "../../../windows/overlapped.h"
#include "../../../windows/queueimpl.h"


namespace Fossilizid{
namespace remoteq{
namespace ipv4{
namespace tcp{

struct channelimpl : public handle{
	channelimpl(QUEUE _que, SOCKET s_){
		_handle_type = handle_ipv4_tcp_channel_type;
		s = s_;
		que = _que;

		windex = 0;
		buflen = 65536;
		buf = (char*)pool::mempool::allocator(buflen);
	}

	QUEUE que;
	container::msque<EVENT> evque;
	binarystream streambuf;

	char * buf;
	int buflen, windex;
	SOCKET s;
};

template<class object, class pack>
bool push(CHANNEL ch, object & obj, pack pack){
	boost::shared_ptr<std::string> buf = pack(object);

	WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
	wsabuf->buf = const_cast<char*>(buf->c_str());
	wsabuf->len = buf->size();
	DWORD bytes = 0;
	overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
	new (ovp)overlappedex();
	ovp->h = (handle*)(channelimpl*)((handle*)ch);
	ovp->type = iocp_type_tcp_send;
	ovp->sendbuf.buf = buf;
	OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovp);
	memset(ovp_, 0, sizeof(OVERLAPPED));
	if (WSASend(((channelimpl*)((handle*)ch))->s, wsabuf, 1, &bytes, 0, ovp_, 0) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING){
			EVENT ev;
			ev.handle.ch = ch;
			ev.type = event_type_reliable_disconnect;
			((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

			return false;
		}
	}

	return true;
}

template<class object, class unpack>
bool pop(CHANNEL ch, object & obj, unpack _unpack){
	channelimpl * implch = (channelimpl*)((handle*)ch);
	
	if (((channelimpl*)((handle*)ch))->que == 0){
		while(1){
			int len = recv(implch->s, implch->buf, implch->windex, 0);
		
			if (len > 0){
				implch->windex += len;
			}

			if (len > 0 && len < buflen){
				break;
			}

			if (len < 0){
				EVENT ev;
				ev.handle.ch = ch;
				ev.type = event_type_reliable_disconnect;
				((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

				break;
			}

			if (len == 0){
				EVENT ev;
				ev.handle.ch = ch;
				ev.type = event_type_reliable_disconnect;
				((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);

				break;
			}

			implch->streambuf.write(implch->buf, implch->windex);
			implch->windex = 0;
		}
	} else{
		if (implch->windex > 0){
			implch->streambuf.write(implch->buf, implch->windex);
			implch->windex = 0;
		}
	}

	auto b = implch->streambuf.read();
	if (b.second > 0){
		int cmdbuflen = 0;
		
		object cmd;
		if ((cmdbuflen = _unpack(cmd, b.first, b.second)) <= 0){
			if (((channelimpl*)((handle*)ch))->que != 0){
				WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
				wsabuf->buf = implch->buf + implch->windex;
				wsabuf->len = implch->buflen - implch->windex;
				DWORD bytes = 0;
				DWORD flags = 0;
				overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
				new (ovp)overlappedex();
				ovp->h = (handle*)(channelimpl*)((handle*)ch);
				ovp->type = iocp_type_tcp_recv;
				OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovp);
				memset(ovp_, 0, sizeof(OVERLAPPED));
				if (WSARecv(implch->s, wsabuf, 1, &bytes, &flags, ovp_, 0) == SOCKET_ERROR){
					if (WSAGetLastError() != WSA_IO_PENDING){
						EVENT ev;
						ev.handle.ch = ch;
						ev.type = event_type_reliable_disconnect;
						((queueimpl*)((handle*)(((channelimpl*)((handle*)ch))->que)))->evque.push(ev);
					}
				}
			}
			return false;
		}
		
		implch->streambuf.seek(cmdbuflen);
	}
		
	return true;
}

} /* namespace tcp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_tcp_channelimpl_h

#endif //_WINDOWS
