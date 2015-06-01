/*
 * queue.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _close_h
#define _close_h

namespace Fossilizid{
namespace remoteq {

typedef void * HANDLE;

/*
 * release handle
 */
void close(HANDLE _handle);

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_close_h