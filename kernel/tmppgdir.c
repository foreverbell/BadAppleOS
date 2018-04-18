// This file should be compiled as C, as non-trivial designated initializers are
// not supported in C++.

#include <stdint.h>

#define HIGH_KERNEL   0xc0000000
#define PAGE_SIZE     4096

#define PAGE_PRESENT  (1 << 0)
#define PAGE_WRITE    (1 << 1)
#define PAGE_HUGE     (1 << 7)

__attribute__((__aligned__(PAGE_SIZE)))
uint32_t tmp_pgdir[1024] = {
  // Map VA [0, 4MB) to PA [0, 4MB).
  [0] = 0x0 | PAGE_PRESENT | PAGE_WRITE | PAGE_HUGE,
  // Map VA [HIGH_KERNEL, HIGH_KERNEL + 4MB) to PA [0, 4MB).
  [HIGH_KERNEL >> 22] = 0x0 | PAGE_PRESENT | PAGE_WRITE | PAGE_HUGE,
};
