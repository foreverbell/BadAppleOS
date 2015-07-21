#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdint.h>

namespace console {

const int video_base = 0xb8000;
const int max_row = 25;
const int max_column = 80;
const int video_size = max_row * max_column;

namespace vga_color {
	
const int black = 0;
const int blue = 1;
const int green = 2;
const int cyan = 3;
const int red = 4;
const int magenta = 5;
const int brown = 6;
const int light_grey = 7;
const int dark_grey = 8;
const int light_blue = 9;
const int light_green = 10;
const int light_cyan = 11;
const int light_red = 12;
const int light_magenta = 13;
const int light_brown = 14;
const int white = 15;

} /* vga_color */

const int default_back_color = vga_color::black;
const int default_fore_color = vga_color::light_grey;

int mkcolor(int, int);
void setcolor(int, bool); // fore, back
void initialize(bool);
void clear(void);
void bkcopy(uint16_t *);
void putch(char);

} /* console */

#endif
