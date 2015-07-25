
#include <stdint.h>
#include <string.h>
#include <system.h>
#include <port.h>
#include <console.h>

namespace console {

/*
 * ------------------> y (80)
 * | > _             |
 * |     console     |
 * |                 |
 * x-----------------x (25)
 */

namespace {

int cursorX, cursorY;
int attrib;
bool has_cursor;

void update_cursor(void) {
	if (has_cursor) {
		int temp = cursorX * VIDEO_MAX_COLUMN + cursorY;

		port::outb(0x3d4, 14);
		port::outb(0x3d5, temp >> 8);
		port::outb(0x3d4, 15);
		port::outb(0x3d5, temp);
	}
}

void sync_cursor(void) {
	if (has_cursor) {
		int offset = 0;

		port::outb(0x3d4, 14);
		offset = port::inb(0x3d5) << 8;
		port::outb(0x3d4, 15);
		offset += port::inb(0x3d5);
		
		cursorX = offset / VIDEO_MAX_COLUMN;
		cursorY = offset % VIDEO_MAX_COLUMN;
	}
}

uint16_t *get_cell_ptr(int row, int col) {
	if (row < 0 || row >= VIDEO_MAX_ROW) {
		return NULL;
	}
	if (col < 0 || col > VIDEO_MAX_COLUMN) {
		return NULL;
	}
	int offset = col + row * VIDEO_MAX_COLUMN;
	uint16_t *ret = (uint16_t *) VIDEO_BASE;
	ret += offset;
	return ret;
}

void scroll(void) {
	uint16_t tofill = 0x20 | (attrib << 8);
	int offset = VIDEO_MAX_COLUMN * 2;

	uint16_t *ptr_first = (uint16_t *) VIDEO_BASE;
	uint16_t *ptr_second = (uint16_t *) (VIDEO_BASE + offset);

	for (int i = 0; i < VIDEO_SIZE - VIDEO_MAX_COLUMN; ++i) {
		*ptr_first++ = *ptr_second++;
	}
	for (int i = 0; i < VIDEO_MAX_COLUMN; ++i) {
		*ptr_first++ = tofill;
	}
}

}

int mkcolor(int fore, int back) {
	return (back << 4) | fore;
}

void setcolor(int color, bool reset) {
	if (reset) {
		uint8_t *ptr = (uint8_t *) VIDEO_BASE;

		ptr += 1;
		for (int i = 0; i < VIDEO_SIZE; ++i) {
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
		port::outb(0x3d4, 0xe);
		port::outb(0x3d5, (2000 >> 8) & 0xff);
		port::outb(0x3d4, 0xf);
		port::outb(0x3d5, 2000 & 0xff);
	}
	sync_cursor();
	
	setcolor(attrib, true);
	clear();
}

void clear(void) {
	uint16_t tofill = 0x20 | (attrib << 8);
	uint16_t *ptr = (uint16_t *) VIDEO_BASE;

	for (int i = 0; i < VIDEO_SIZE; ++i) {
		*ptr++ = tofill;
	}

	cursorX = cursorY = 0;
	update_cursor();
}

void bkcopy(const uint16_t *src) {
	memcpy((void *) VIDEO_BASE, (void *) src, VIDEO_SIZE * 2);
	
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
	if (cursorY >= VIDEO_MAX_COLUMN) {
		cursorY = 0;
		cursorX += 1;
	}
	while (cursorX >= VIDEO_MAX_ROW) {
		scroll();
		cursorX -= 1;
	}
	update_cursor();
}

} /* console */
