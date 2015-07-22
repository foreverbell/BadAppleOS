
#include <system.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "video.h"

namespace badapple {

extern uint8_t vdatas[] asm("_binary_vdata_bin_start");
extern uint8_t vdatae[] asm("_binary_vdata_bin_end");

video::video(void) {
	printf("video raw data address = 0x%x.\n", (int) vdatas);

	cur_frame = 0;
	count = int(vdatae - vdatas) * 8 / console::video_size;
	// pool = (uint16_t *) malloc(count * console::video_size * 2);
	pool = new uint16_t[count * console::video_size];
	
	printf("video frame count = %d.\n", count);
	printf("video pool address = 0x%x.\n", (int) pool);
	
	uint8_t *raw_ptr = (uint8_t *) vdatas;
	uint16_t *pool_ptr = pool;
	uint16_t attrib;
	
	while (raw_ptr < vdatae) {
		uint8_t raw = *raw_ptr;
		for (int i = 0; i < 8; ++i) {
			attrib = (0xf << 8) | (raw & 1 ? ' ' : '*');
			raw = raw >> 1;
			*pool_ptr++ = attrib;
		}
		++raw_ptr;
	}
	
	printf("video data loaded, tailing pointer 0x%x.\n", (int) pool_ptr);
}

int video::progress(void) const {
	return (cur_frame + 1) * 100 / count;
}

bool video::has_next(void) const {
	return cur_frame < count;
}

void video::next(void) {
	console::bkcopy(pool + cur_frame * console::video_size);
	cur_frame += 1;
}

void video::free(void) {
	delete [] pool;
	// free(pool);
}

} /* badapple */