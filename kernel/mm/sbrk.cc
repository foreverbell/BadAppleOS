
#include <stdint.h>
#include <stdio.h>

namespace mm {
	
namespace {

void * const MFAIL = (void *) ~(size_t(0));

uint8_t * const kernel_mem_begin = (uint8_t *) 0xC0400000;
uint8_t * const kernel_mem_end   = (uint8_t *) 0xC0C00000;

uint8_t *pbreak = kernel_mem_begin;

}

// implementation follows http://linux.die.net/man/2/sbrk.
void *sbrk(ptrdiff_t increment) {
	auto ptr = pbreak;
	
	printf("[sbrk] increment: 0x%x\n", increment);
	if (increment < 0 || pbreak + increment > kernel_mem_end) {
		printf("[sbrk] MFAIL\n");
		return MFAIL;
	}
	if (increment > 0) {
		pbreak += increment;
	}
	return ptr;
}

} /* mm */
