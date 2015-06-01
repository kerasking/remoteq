/*
 * endpointimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _endpointimpl_h
#define _endpointimpl_h

#include "../../windows/handle.h"

namespace Fossilizid{
namespace remoteq{
namespace ipv4{

struct endpointimpl : public handle{
	endpointimpl(){
		_handle_type = handle_ipv4_endpoint_type;
	}

	SOCKADDR_IN addr;
	int len;

};

}

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_endpointimpl_h

#endif //_WINDOWS