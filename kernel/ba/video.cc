
#include <system.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "video.h"

namespace badapple {

extern uint8_t vdatas[] asm("_binary_vdata_bin_start");
extern uint8_t vdatae[] asm("_binary_vdata_bin_end");

video::video(void) {
	printf("video raw data address = 0x%x.\n", (int) vdatas);
	
	cur_frame = count = 0;
	pool = (uint16_t *) 0xC0400000;
		
	uint8_t *raw_ptr = (uint8_t *) vdatas;
	uint16_t *pool_ptr = pool;
	uint16_t attrib;
	
	count = int(vdatae - vdatas) * 8 / console::video_size;
	printf("video frame count = %d.\n", count);
	
	while (raw_ptr < vdatae) {
		uint8_t raw = *raw_ptr;
		for (int i = 0; i < 8; ++i) {
			attrib = (0xf << 8) | (raw & 1 ? ' ' : '*');
			raw = raw >> 1;
			*pool_ptr++ = attrib;
		}
		++raw_ptr;
	}
	
	printf("video data loaded.\n");
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

} /* badapple */