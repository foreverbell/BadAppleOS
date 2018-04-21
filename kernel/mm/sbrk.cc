
#include <stdint.h>
#include <stdio.h>
#include "sbrk.h"

namespace mm {

namespace {

void* const MFAIL = (void*) ~(size_t(0));

#define KRNL_MEM_BEGIN ((void*) 0xc0400000)
#define KRNL_MEM_END   ((void*) 0xc0c00000)

uint8_t* pbreak = (uint8_t*) KRNL_MEM_BEGIN;

}

// implementation follows http://linux.die.net/man/2/sbrk.
void *sbrk(ptrdiff_t increment) {
  auto ptr = pbreak;
  auto new_ptr = pbreak + increment;

  printf("[sbrk] increment: 0x%x.\n", increment);
  if (new_ptr < KRNL_MEM_BEGIN || new_ptr > KRNL_MEM_END) {
    puts("[sbrk] MFAIL.");
    return MFAIL;
  }
  pbreak = new_ptr;
  return ptr;
}

} /* mm */
