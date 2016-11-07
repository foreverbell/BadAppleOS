
#include <cpu.h>
#include <system.h>
#include <panic.h>
#include <assert.h>
#include <timer.h>

namespace cpu {

namespace manipulate {

namespace {

int cli_count = 1;

}

void cli(void) {
  cli_count += 1;
  __asm__ __volatile__ ("cli");
}

void sti(void) {
  cli_count -= 1;
  assert(cli_count >= 0);
  if (cli_count == 0) {
    __asm__ __volatile__ ("sti");
  }
}

void halt(void) {
  __asm__ __volatile__ ("hlt");
}

/* clear interrupts and halt. */
void die(void) {
  cli();
  halt();
}

void sleep(uint64_t ticks) {
  uint64_t till = ticks + timer::get_system_tick();
  while (timer::get_system_tick() < till) {
    halt();
  }
}

/* impl of int_guard. */
int_guard::int_guard() : cleared(false) {
  acquire();
}

int_guard::~int_guard() {
  release();
}

void int_guard::acquire(void) {
  if (cleared) {
    panic::panic("[int_guard::acquire] Already acquired.");
  }
  cli();
  cleared = true;
}

void int_guard::release(void) {
  if (!cleared) {
    panic::panic("[int_guard::release] Nothing to release.");
  }
  cleared = false;
  sti();
}

bool int_guard::owns(void) const {
  return cleared;
}

} /* manipulate */

} /* cpu */
