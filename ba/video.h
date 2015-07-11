#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>

namespace badapple {
	
class video {
public:
	video();
	void next();
	bool has_next();
private:
	uint16_t *pool;
	int count, cur_frame;
};

} /* badapple */

#endif