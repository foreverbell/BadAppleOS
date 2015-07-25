#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>
#include <vector>
#include <pair>

namespace badapple {
	
class video {
public:
	video();
	int progress(void) const;
	void next(void);
	bool has_next(void) const;
	void free(void);
private:
	void artify(void);
	char at(int, int, int) const;
	void set(int, int, int, char);
	
	std::pair<int, char> find(int);
	void join(int, int);
private:
	uint16_t *pool;
	int count, cur_frame;
	std::vector<std::pair<int, char>> dsu;
};

} /* badapple */

#endif