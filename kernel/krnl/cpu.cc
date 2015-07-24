
#include <system.h>
#include <assert.h>

namespace cpu {
	
namespace {
	
int cli_count;

}

void initialize(void) {
	cli_count = 1;
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

} /* cpu */