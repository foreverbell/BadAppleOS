
#include <timer.h>
#include <cpu.h>
#include <port.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <systime.h>
#include <new>

using cpu::manipulate::int_guard;

namespace timer {

namespace {

#define PORT_PIT_CHANNEL0   0x40
#define PORT_PIT_CHANNEL1   0x41
#define PORT_PIT_CHANNEL2   0x42
#define PORT_PIT_CMD        0x43

struct timer_callback_node_t {
  /* all measured in tick, not second. */
  uint64_t interval, trigger_count, count_down;
  timer_callback_node_t *prev, *next;
  fn_timer_callback_t lpfn_timer_callback;
  bool deleted;
} *timer_head;

uint64_t system_tick;

void handler(irq::irq_context_t * /*context_ptr*/) {
  system_tick += 1;

  timer_callback_node_t *cur = timer_head;

  while (cur != NULL) {
    cur->count_down -= 1;

    bool should_remove = cur->deleted;
    bool should_call = !cur->deleted && cur->count_down == 0;

    if (should_call) {
      cur->lpfn_timer_callback(cur->trigger_count, (handle_t) cur);
      cur->trigger_count += 1;
      cur->count_down = cur->interval;
    }
    if (should_remove) {
      if (timer_head == cur) {
        timer_head = cur->next;
      }
      if (cur->prev != NULL) {
        cur->prev->next = cur->next;
      }
      if (cur->next != NULL) {
        cur->next->prev = cur->prev;
      }
      timer_callback_node_t *next = cur->next;
      delete cur;
      cur = next;
    } else {
      cur = cur->next;
    }
  }
}

}

void initialize(void) {
  system_tick = 0;

  /* clear timer link head. */
  timer_head = NULL;

  /* default tick rate, 18 ticks = 1 second. */
  port::outb(PORT_PIT_CMD, 0x36);
  port::outb(PORT_PIT_CHANNEL0, 0);
  port::outb(PORT_PIT_CHANNEL0, 0);

  /* install and enable coresponding IRQ. */
  irq::install(0, handler);
  irq::enable(0);
}

uint64_t get_system_tick(void) {
  return system_tick;
}

handle_t add(uint64_t interval, fn_timer_callback_t lpfn_callback) {
  if (interval == 0) {
    return TIMER_INVALID_HANDLE;
  }

  int_guard guard;
  timer_callback_node_t *ptr = new timer_callback_node_t();

  if (ptr == NULL) {
    return TIMER_INVALID_HANDLE;
  }

  ptr->deleted = false;
  ptr->prev = NULL;
  ptr->next = timer_head;
  if (timer_head != NULL) {
    timer_head->prev = ptr;
  }
  ptr->interval = interval;
  ptr->trigger_count = 0;
  ptr->count_down = interval;
  ptr->lpfn_timer_callback = lpfn_callback;
  timer_head = ptr;

  return (handle_t) ptr;
}

bool remove(handle_t ptr) {
  int_guard guard;
  timer_callback_node_t *timer_ptr = (timer_callback_node_t *) ptr;

  if (timer_ptr->deleted) {
    return false;
  }
  timer_ptr->deleted = true;
  return true;
}

} /* timer */
