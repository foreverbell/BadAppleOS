#ifndef __BADAPPLE_H__
#define __BADAPPLE_H__

#include <stdint.h>
#include <vector>
#include <pair>

namespace badapple {

class stream {
public:
	stream(const uint8_t *, const uint8_t *);
	int nextb();
	bool has_next() const;
	int remain() const;
private:
	const uint8_t *pool, *pool_end, *seek;
	int shift;
};

class decompressor {
public:
	decompressor(const uint8_t *vdatae, const uint8_t *vdatas);
	~decompressor();
	
	int frame_count() const { return count; }
	const uint8_t *data() const { return buffer; }
	const uint8_t *data_end() const { return buffer_end; }
	
	decompressor(const decompressor &) = delete;
	decompressor &operator = (const decompressor &) = delete;
private:
	int count;
	uint8_t *buffer, *buffer_end;
};

class video {
public:
	video();
	int progress() const;
	void next();
	bool has_next() const;
	void free();
private:
	void artify();
	
	/* dsu */
	struct info_t {
		char ch;
		int size;
	};
	std::pair<int, info_t> find(int);
	void join(int, int);
private:
	uint16_t *pool;
	int count, cur_frame;
	std::vector<std::pair<int, info_t>> dsu;
};

} /* badapple */

#endif
