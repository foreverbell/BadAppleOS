#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdint.h>

namespace console {

#define VIDEO_BASE        0xb8000
#define VIDEO_MAX_ROW     25
#define VIDEO_MAX_COLUMN  80
#define VIDEO_SIZE        2000 // 25*80

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

int mkcolor(int, int);  // fore, back
void setcolor(int, bool);
void initialize(bool);
void clear(void);
void bkcopy(const uint16_t *);
void putch(char);

} /* console */

#endif
