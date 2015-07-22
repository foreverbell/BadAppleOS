#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>

namespace badapple {
	
class video {
public:
	video();
	int progress(void) const;
	void next(void);
	bool has_next(void) const;
	void free(void);
private:
	uint16_t *pool;
	int count, cur_frame;
};

} /* badapple */

#endif