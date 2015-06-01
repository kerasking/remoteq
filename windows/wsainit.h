/*
 * wsainit.h
 *
 *  Created on: 2014-10-21
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#include <Mswsock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#ifndef _wsainit_h
#define _wsainit_h

namespace Fossilizid{
namespace remoteq {

struct init{
	init(){
		WSADATA _WSADATA;
		WSAStartup(MAKEWORD(2, 2), &_WSADATA);
	}

	~init(){
		WSACleanup();
	}
};

static init _init;


} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_wsainit_h

#endif //_WINDOWS