
#include <system.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <new>
#include <vector>
#include <pair>
#include "video.h"

using std::vector;
using std::pair;
using std::make_pair;

namespace badapple {

extern uint8_t vdatas[] asm("_binary_vdata_bin_start");
extern uint8_t vdatae[] asm("_binary_vdata_bin_end");

video::video(void) {
#ifdef VIDEO_SQUARE_TEXT
	const uint16_t bkcolor1 = console::mkcolor(console::vga_color::white, console::vga_color::black);
	const uint16_t bkcolor2 = console::mkcolor(console::vga_color::black, console::vga_color::white);
#else
	const uint16_t acolor = console::mkcolor(console::vga_color::white, console::vga_color::black);
#endif

	printf("[video] Raw data address = 0x%x.\n", (int) vdatas);

	cur_frame = 0;
	count = int(vdatae - vdatas) * 8 / VIDEO_SIZE;
	// pool = (uint16_t *) malloc(count * VIDEO_SIZE * 2);
	pool = new uint16_t[count * VIDEO_SIZE];
	
	printf("[video] Frame count = %d.\n", count);
	printf("[video] Pool address = 0x%x.\n", (int) pool);
	
	uint8_t *raw_ptr = (uint8_t *) vdatas;
	uint16_t *pool_ptr = pool;
	uint16_t attrib;
	
	while (raw_ptr < vdatae) {
		uint8_t raw = *raw_ptr;
		for (int i = 0; i < 8; ++i) {
#ifdef VIDEO_SQUARE_TEXT
			attrib = (raw & 1 ? bkcolor1 : bkcolor2) << 8;
#else
			attrib = (acolor << 8) | (raw & 1 ? ' ' : '#');
#endif
			raw = raw >> 1;
			*pool_ptr++ = attrib;
		}
		++raw_ptr;
	}
	printf("[video] Data loaded, tailing pointer = 0x%x.\n", (int) pool_ptr);

#ifndef VIDEO_SQUARE_TEXT
	puts("[video] Artifying data, this may take a few second.");
	artify();
	puts("[video] Data artified.");
#endif
}

char video::at(int frame, int x, int y) const {
	const uint16_t *p = pool;
	p += frame * VIDEO_SIZE;
	p += y * VIDEO_MAX_COLUMN;
	p += x;
	return *(const char *) p;
}

void video::set(int frame, int x, int y, char ch) {
	const uint16_t *p = pool;
	p += frame * VIDEO_SIZE;
	p += y * VIDEO_MAX_COLUMN;
	p += x;
	*(char *) p = ch;
}

std::pair<int, char> video::find(int x) {
	if (dsu[x].first != x) {
		dsu[x] = find(dsu[x].first);
	}
	return dsu[x];
}

void video::join(int x, int y) {
	x = find(x).first;
	y = find(y).first;
	if (dsu[x].second == '?') {
		dsu[x] = dsu[y];
	} else {
		dsu[y] = dsu[x];
	}
}

void video::artify(void) {
	const int dxy[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	
	const char dot_chars[4] = {'\'', '`', ',', '.'};
	const char line_chars[4] = {'\\', '/', char(28), '^'};
	const char dense_chars[7] = {'M', 'H', 'W', '#', '*', '&', '@'};
	
	dsu.resize(VIDEO_SIZE);
	
	auto label = [&](int x, int y) -> int {
		return y * VIDEO_MAX_COLUMN + x;
	};
	auto within = [&](int x, int y) -> bool {
		return x >= 0 && x < VIDEO_MAX_COLUMN && y >= 0 && y < VIDEO_MAX_ROW;
	};
	
	/* randomly change connected block to one dense char. */
	for (int f = 0; f < count; ++f) {
		for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
			for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
				char ch = at(f, x, y);
				if (ch != ' ') {
					if (f > 0 && at(f - 1, x, y) != ' ') {
						/* 1% possibility to get a mutation. */
						ch = rand() % 100 == 0 ? dense_chars[rand() % 7] : at(f - 1, x, y);
					} else {
						ch = '?'; // to be determinated.
					}
				}
				dsu[label(x, y)] = make_pair(label(x, y), ch);
			}
		}
		for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
			for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
				if (at(f, x, y) == ' ') {
					continue;
				}
				for (int d = 0; d < 4; ++d) {
					if (!within(x + dxy[d][0], y + dxy[d][1]) || at(f, x + dxy[d][0], y + dxy[d][1]) == ' ') {
						continue;
					}
					join(label(x, y), label(x + dxy[d][0], y + dxy[d][1]));
				}
			}
		}
		for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
			for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
				char &ch = dsu[find(label(x, y)).first].second;
				if (ch == '?') {
					ch = dense_chars[rand() % 7];
				}
				set(f, x, y, ch);
			}
		}
	}
	
	/* make boundary more variety. */
	for (int f = 0; f < count; ++f) {
		for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
			for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
				if (at(f, x, y) == ' ') {
					continue;
				}
				int dir = -1, empty = 0;
				for (int d = 0; d < 4; ++d) {
					if (!within(x + dxy[d][0], y + dxy[d][1])) {
						continue;
					}
					if (at(f, x + dxy[d][0], y + dxy[d][1]) == ' ') {
						dir = d;
						empty += 1;
					}
				}
				if (dir != -1) {
					char to_replace = dot_chars[dir];
					if (empty <= 2 && 
						at(f, x + dxy[(3 - dir) ^ 1][0], y + dxy[(3 - dir) ^ 1][1]) != ' ' &&
						at(f, x + dxy[3 - dir][0], y + dxy[3 - dir][1]) != ' ') {
							to_replace = line_chars[dir];
					}
					set(f, x, y, to_replace);
				}
			}
		}
	}
}

int video::progress(void) const {
	return (cur_frame + 1) * 100 / count;
}

bool video::has_next(void) const {
	return cur_frame < count;
}

void video::next(void) {
	console::bkcopy(pool + cur_frame * VIDEO_SIZE);
	cur_frame += 1;
}

void video::free(void) {
	delete [] pool;
	// free(pool);
}

} /* badapple */