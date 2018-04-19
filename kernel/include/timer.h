#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

namespace timer {

#define TIMER_INVALID_HANDLE  NULL

#define TIMER_TICK_PER_SECOND 18

typedef void* handle_t;
typedef void (* fn_timer_callback_t)(uint64_t, handle_t);

void initialize(void);
uint64_t get_system_tick(void);
handle_t add(uint64_t, fn_timer_callback_t);
bool remove(handle_t);

} /* timer */

#endif
