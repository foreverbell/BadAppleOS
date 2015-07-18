#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdint.h>

namespace console {

void initialize(bool);
void set_color(int);
void clear(void);
void bkcopy(uint16_t *);
void putch(char);

} /* console */

#endif
