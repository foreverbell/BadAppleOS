#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

namespace timer {

#define TIMER_TICK_ONE_SECOND 18
#define TIMER_TICK_ONE_SHOT   0

typedef void (* fn_timer_callback_t)(uint64_t, int);

void initialize(void);
uint64_t get_system_tick(void);
int add(uint64_t, fn_timer_callback_t);
bool remove(int);

} /* timer */

#endif