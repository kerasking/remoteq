/*
 * queue.h
 *
 *  Created on: 2015-5-17
 *      Author: qianqians
 */
#ifndef _binarystream_h
#define _binarystream_h

#include <pool/mempool.h>

namespace Fossilizid{
namespace remoteq {

class binarystream{
public:
	binarystream(){
		data = 0;
		rindex = 0;
		len = 0;
		windex = 0;
	}

	~binarystream(){
		pool::mempool::deallocator(data, len);
	}

	void write(char * buf, size_t blen){
		if ((len - windex) <= blen){
			len *= 2;
			char * tmp = (char*)pool::mempool::allocator(len);
			memcpy(tmp, data, windex);
			data = tmp;
		}
		memcpy(data + windex, buf, blen);
		windex += blen;
	}

	std::pair<char *, size_t> read(){
		return std::make_pair(data + rindex, windex - rindex);
	}

	void seek(size_t offset){
		rindex += offset;
	}

	void clear(){
		rindex = 0;
		len = 0;
		windex = 0;
	}

private:
	char * data;
	size_t len;
	size_t windex;
	size_t rindex;

};

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_binarystream_h