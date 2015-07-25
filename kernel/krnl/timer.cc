
#include <system.h>
#include <cpu_manipulate.h>
#include <port.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <systime.h>

namespace timer {

namespace {

#define MAX_TIMER_CALLBACK 16

struct timer_callback_link_t {
	int identifier;
	/* all measured in tick, not second. */
	uint64_t interval, trigger_count, count_down;
	timer_callback_link_t *prev, *next;
	fn_timer_callback_t lpfn_timer_callback;
	bool deleted;
} timer_pool[MAX_TIMER_CALLBACK], *timer_head;

uint64_t system_tick;

void handler(irq::irq_context_t * /*context_ptr*/) {
	static uint32_t ticks = 0;

	system_tick += 1;

	timer_callback_link_t *cur = timer_head;
	
	while (cur != NULL) {
		if (cur->interval != TIMER_TICK_ONE_SHOT) {
			cur->count_down -= 1;
		}
		
		bool to_remove = (cur->deleted || cur->interval == TIMER_TICK_ONE_SHOT);
		bool to_call = !cur->deleted && (cur->count_down == 0 || cur->interval == TIMER_TICK_ONE_SHOT);
		
		if (to_call) {
			cur->lpfn_timer_callback(cur->trigger_count, cur->identifier);
			cur->trigger_count += 1;
			cur->count_down = cur->interval;
		}
		if (to_remove) {
			if (timer_head == cur) {
				timer_head = cur->next;
			}
			if (cur->prev != NULL) {
				cur->prev->next = cur->next;
			}
			if (cur->next != NULL) {
				cur->next->prev = cur->prev;
			}
			timer_callback_link_t *next = cur->next;
			memset(cur, 0, sizeof(timer_callback_link_t));
			cur = next;
		} else {
			cur = cur->next;
		}
	}
	
	if (++ticks >= TIMER_TICK_ONE_SECOND) {
		// printf("One second has passed.\n");
		ticks = 0;
	}
}

}

void initialize(void) {
	system_tick = 0;

	/* clear timer pool. */
	memset(timer_pool, 0, sizeof(timer_pool));
	timer_head = 0;
	
	/* default tick rate, 18 ticks = 1 second. */
	port::outb(PORT_PIT_COMMAND, 0x36);
	port::outb(PORT_PIT_CHANNEL0, 0);
	port::outb(PORT_PIT_CHANNEL0, 0);

	/* install and enable corespond IRQ. */
	irq::install(0, handler);
	irq::enable(0);
}

uint64_t get_system_tick(void) {
	return system_tick;
}

int add(uint64_t interval, fn_timer_callback_t lpfn_callback) {
	cpu::int_guard guard;
	
	for (int i = 0; i < MAX_TIMER_CALLBACK; ++i) {
		if (timer_pool[i].lpfn_timer_callback == NULL) {
			timer_pool[i].deleted = false;
			timer_pool[i].prev = NULL;
			timer_pool[i].next = timer_head;
			if (timer_head != NULL) {
				timer_head->prev = timer_pool + i;
			}
			timer_pool[i].identifier = i;
			timer_pool[i].interval = interval;
			timer_pool[i].trigger_count = 0;
			timer_pool[i].count_down = interval;
			timer_pool[i].lpfn_timer_callback = lpfn_callback;
			timer_head = timer_pool + i;
			return i;
		}
	}
	return -1;
}

bool remove(int identifier) {
	if (timer_pool[identifier].lpfn_timer_callback == NULL) {
		return false;
	}
	timer_pool[identifier].deleted = true;
	return true;
}

} /* timer */ 
