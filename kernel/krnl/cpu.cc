
#include <cpu_manipulate.h>
#include <system.h>
#include <panic.h>
#include <assert.h>

namespace cpu {
	
namespace {
	
int cli_count = 1;

}

void initialize(void) {
	/* empty stub */
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
	cpu::cli();
	cleared = true;
}

void int_guard::release(void) {
	if (!cleared) {
		panic::panic("[int_guard::release] Nothing to release.");
	}
	cleared = false;
	cpu::sti();
}

bool int_guard::owns(void) const {
	return cleared;
}

} /* cpu */