/*
 * queueimpl.h
 *
 *  Created on: 2014-10-5
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#ifndef _ipv4_udp_queueimpl_h
#define _ipv4_udp_queueimpl_h

#include "../ipv4/windows/acceptorimpl.h"
#include "../ipv4/windows/channelimpl.h"
#include "../ipv4/windows/endpointimpl.h"

namespace Fossilizid{
namespace remoteq {
namespace ipv4{
namespace udp {

	void dispense(uint16_t type, queueimpl * impl, EVENT & ev, DWORD bytes, handle * h){
		acceptorimlp * acp = (acceptorimlp*)h;
		channelimpl * ch = 0;
		uint32_t addr = ((endpointimpl*)acp->from)->addr.sin_addr.S_un.S_addr;
		uint32_t port = ((endpointimpl*)acp->from)->addr.sin_port;
		uint64_t key = (addr | (uint64_t)(port) << 32);
		auto find = acp->channels.find(key);
		if (find == acp->channels.end()){
			ev.type = event_type_ipv4_udp_accept;
			ev.handle.acp = (ACCEPTOR)(acp);

			ch = pool::objpool<channelimpl>::allocator(1);
			new (ch)channelimpl(acp->que, acp->s, endpoint(inet_ntoa(((endpointimpl*)acp->from)->addr.sin_addr), ((endpointimpl*)acp->from)->addr.sin_port));
			ch->acp = acp;
			acp->channels.insert(std::make_pair(key, ch));
			acp->chque.push(ch);

			EVENT recvev;
			recvev.type = event_type_ipv4_udp_recv;
			recvev.handle.ch = (CHANNEL)(ch);
			((queueimpl *)acp->que)->evque.push(recvev);

		} else{
			ch = (channelimpl *)find->second;

			ev.type = event_type_ipv4_udp_recv;
			ev.handle.ch = (CHANNEL)(ch);
		}

		if (bytes >= DWORD(ch->buflen - ch->windex)){
			auto buflen = ch->buflen;
			ch->buflen *= 2;
			char * tmp = ch->buf;
			ch->buf = (char*)pool::mempool::allocator(ch->buflen);
			memcpy(ch->buf, tmp, buflen);
			pool::mempool::deallocator(tmp, buflen);
		}
		memmove(ch->buf + ch->windex, acp->outbuf, bytes);
		ch->windex += bytes;
	}

} /* namespace udp */
} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_ipv4_udp_queueimpl_h

#endif //_WINDOWS