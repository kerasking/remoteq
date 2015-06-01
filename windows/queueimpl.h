/*
 * queueimpl.h
 *
 *  Created on: 2014-10-5
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _queueimpl_h
#define _queueimpl_h

#include <container/msque.h>

#include "handle.h"
#include "../queue.h"

namespace Fossilizid{
namespace remoteq {


struct queueimpl : public handle{
	queueimpl(){
		_handle_type = handle_queue_type;
	}

	~queueimpl(){
	}

	container::msque<EVENT> evque;
	HANDLE iocp;
};

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_queueimpl_h

#endif //_WINDOWS