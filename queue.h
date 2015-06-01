/*
 * queue.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _queue_h
#define _queue_h

#include "typedef.h"

namespace Fossilizid{
namespace remoteq {

/*
 * create a QUEUE
 */
QUEUE queue();

enum event_type{
	event_type_none,
	event_type_ipv4_tcp_accept,
	event_type_ipv4_tcp_recv,
	event_type_ipv4_tcp_disconnect,
	event_type_ipv4_udp_accept,
	event_type_ipv4_udp_recv,
	event_type_ipv4_udp_disconnect,
	event_size,
};

struct EVENT{
	union { ACCEPTOR acp; CHANNEL ch; } handle;
	event_type type;
};

/*
 * get EVENT from QUEUE
 */
EVENT queue(QUEUE que);

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_queue_h