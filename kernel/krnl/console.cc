
#include <stdint.h>
#include <string.h>
#include <system.h>
#include <port.h>
#include <console.h>

namespace console {

/*
 * ------------> y (80)
 * |           |
 * |  console  |
 * |           |
 * x-----------x
 */

static int cursorX, cursorY;
static int attrib;
static bool has_cursor;

static void update_cursor(void) {
	if (has_cursor) {
		int temp = cursorX * max_column + cursorY;

		outportb(0x3D4, 14);
		outportb(0x3D5, temp >> 8);
		outportb(0x3D4, 15);
		outportb(0x3D5, temp);
	}
}

static void sync_cursor(void) {
	if (has_cursor) {
		int offset = 0;

		outportb(0x3D4, 14);
		offset = inportb(0x3D5) << 8;
		outportb(0x3D4, 15);
		offset += inportb(0x3D5);
		
		cursorX = offset / max_column;
		cursorY = offset % max_column;
	}
}

static uint16_t *get_cell_ptr(int row, int col) {
	if (row < 0 || row >= max_row) {
		return NULL;
	}
	if (col < 0 || col > max_column) {
		return NULL;
	}
	int offset = col + row * max_column;
	uint16_t *ret = (uint16_t *) video_base;
	ret += offset;
	return ret;
}

static void scroll(void) {
	uint16_t tofill = 0x20 | (attrib << 8);
	int offset = max_column * 2;

	uint16_t *ptr_first = (uint16_t *) video_base;
	uint16_t *ptr_second = (uint16_t *) (video_base + offset);

	for (int i = 0; i < video_size - max_column; ++i) {
		*ptr_first++ = *ptr_second++;
	}
	for (int i = 0; i < max_column; ++i) {
		*ptr_first++ = tofill;
	}
}

int mkcolor(int fore, int back) {
	return (back << 4) | fore;
}

void setcolor(int color, bool reset) {
	if (reset) {
		uint8_t *ptr = (uint8_t *) video_base;

		ptr += 1;
		for (int i = 0; i < video_size; ++i) {
			*ptr = color;
			ptr += 2;
		}
	}
	attrib = color;
}

void initialize(bool cursor) {
	has_cursor = cursor;
	attrib = mkcolor(default_fore_color, default_back_color);
	
	if (!has_cursor) {
		outportb(0x3d4, 0xe);
		outportb(0x3d5, (2000 >> 8) & 0xff);
		outportb(0x3d4, 0xf);
		outportb(0x3d5, 2000 & 0xff);
	}
	sync_cursor();
	
	setcolor(attrib, true);
}

void clear(void) {
	uint16_t tofill = 0x20 | (attrib << 8);
	uint16_t *ptr = (uint16_t *) video_base;

	for (int i = 0; i < video_size; ++i) {
		*ptr++ = tofill;
	}

	cursorX = cursorY = 0;
	update_cursor();
}

void bkcopy(uint16_t *src) {
	memcpy((void *) video_base, src, video_size*2);
	
	cursorX = cursorY = 0;
	update_cursor();
}

void putch(char ch) {
	uint16_t *ptr = get_cell_ptr(cursorX, cursorY);

	if (ptr == NULL) {
		return;
	}

	if (ch == '\b') { // backspace
		if (cursorY != 0) {
			cursorY -= 1;
		}
	} else if (ch == '\t') { // tab
		cursorY = (cursorY + 4) & ~3;
	} else if (ch == '\r') {
		cursorY = 0;
	} else if (ch == '\n') {
		cursorY = 0;
		cursorX += 1;
	} else {
		*ptr = ch | (attrib << 8);
		cursorY += 1;
	}
	if (cursorY >= max_column) {
		cursorY = 0;
		cursorX += 1;
	}
	while (cursorX >= max_row) {
		scroll();
		cursorX -= 1;
	}
	update_cursor();
}

} /* console */
