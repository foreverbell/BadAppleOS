#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>

namespace badapple {
	
class video {
public:
	video();
	int progress() const;
	void next();
	bool has_next() const;
private:
	uint16_t *pool;
	int count, cur_frame;
};

} /* badapple */

#endif