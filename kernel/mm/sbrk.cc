
#include <stdint.h>
#include <stdio.h>
#include "sbrk.h"

namespace mm {
	
namespace {

void * const MFAIL = (void *) ~(size_t(0));

#define KERNEL_MEM_BEGIN (void *) 0xC0400000
#define KERNEL_MEM_END   (void *) 0xC0C00000

uint8_t *pbreak = (uint8_t *) KERNEL_MEM_BEGIN;

}

// implementation follows http://linux.die.net/man/2/sbrk.
void *sbrk(ptrdiff_t increment) {
	auto ptr = pbreak;
	
	printf("[sbrk] increment: 0x%x.\n", increment);
	if (increment < 0 || pbreak + increment > KERNEL_MEM_END) {
		puts("[sbrk] MFAIL.");
		return MFAIL;
	}
	if (increment > 0) {
		pbreak += increment;
	}
	return ptr;
}

} /* mm */
