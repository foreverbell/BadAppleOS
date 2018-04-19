
#include <stdint.h>
#include <stdio.h>
#include "sbrk.h"

namespace mm {

namespace {

void * const MFAIL = (void *) ~(size_t(0));

#define KERNEL_MEM_BEGIN 0xc0400000
#define KERNEL_MEM_END   0xc0c00000

uint8_t *pbreak = (uint8_t *) KERNEL_MEM_BEGIN;

}

// implementation follows http://linux.die.net/man/2/sbrk.
void *sbrk(ptrdiff_t increment) {
  auto ptr = pbreak;
  auto new_ptr = pbreak + increment;

  printf("[sbrk] increment: 0x%x.\n", increment);
  if (increment < 0 || new_ptr > (void *) KERNEL_MEM_END) {
    puts("[sbrk] MFAIL.");
    return MFAIL;
  }
  if (increment > 0) {
    pbreak += increment;
  }
  return ptr;
}

} /* mm */
