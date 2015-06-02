/*
 * endpoint.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _endpoint_h
#define _endpoint_h

#include "typedef.h"

#include <string>

namespace Fossilizid{
namespace remoteq {
namespace ipv4 {

/*
 * create a endpoint
 */
ENDPOINT endpoint(char * ip, short port);

/*
 * get remote endpoint 
 */
ENDPOINT remotep(CHANNEL ch);

/*
 * get endpoint ip
 */
std::string ip(ENDPOINT ep);

/*
 * get endpoint port
 */
short port(ENDPOINT ep);

} /* namespace ipv4 */
} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_endpoint_h