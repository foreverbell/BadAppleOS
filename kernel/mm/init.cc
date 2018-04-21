
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <port.h>

namespace mm {

#define NVRAM_BASELO   0x15
#define NVRAM_BASEHI   0x16
#define NVRAM_EXTLO    0x17
#define NVRAM_EXTHI    0x18
#define NVRAM_EXT16LO  0x34
#define NVRAM_EXT16HI  0x35

#define PAGE_PRESENT  (1 << 0)
#define PAGE_WRITE    (1 << 1)

// To be simple, we use physical memory of [0x1000, 0x6000) for PDE and PTE.
// PDE and each PTE occupy one page.

#define PDE   0x1000
#define PTE1  0x2000
#define PTE2  0x3000
#define PTE3  0x4000
#define PTE4  0x5000

namespace {

uint32_t * const pde_addr = (uint32_t *) PDE;

/* map VA `index*4M+4K*[from, to)` to PA `phys+4K*[from, to)`. */
void setup_pte(int index, uint32_t pte_addr, uint32_t phys, int from, int to) {
  pde_addr[index] = pte_addr | PAGE_PRESENT | PAGE_WRITE;
  uint32_t *p = (uint32_t *) pte_addr;

  for (int i = 0; i < 1024; ++i) {
    p[i] = 0;
    if (i >= from && i < to) {
      p[i] = ((i << 12) + phys) | PAGE_PRESENT | PAGE_WRITE;
    }
  }
}

}

void initialize(void) {
  auto read = [](int r) -> size_t { return cmos::read(r); };
  size_t basemem, extmem, ext16mem, totalmem;

  /* use CMOS calls to measure available base & extended memory. */
  /* measured in kilobytes. */
  basemem = read(NVRAM_BASELO) | (read(NVRAM_BASEHI) << 8);
  extmem = read(NVRAM_EXTLO) | (read(NVRAM_EXTHI) << 8);
  ext16mem = (read(NVRAM_EXT16LO) | (read(NVRAM_EXT16HI) << 8)) << 6;

  if (ext16mem) {
    totalmem = 16 * 1024 + ext16mem;
  } else if (extmem) {
    totalmem = 1 * 1024 + extmem;
  } else {
    totalmem = basemem;
  }

  printf("[mm] Physical total = %uK, base = %uK, extended = %uK.\n",
         totalmem, basemem, totalmem - basemem);

  /* reinitialize PDE and PTE. */

  /* clear all PDE entries. */
  for (int i = 0; i < 1024; ++i) {
    pde_addr[i] = 0;
  }

  /* set lower 1M memory as identity paging. */
  setup_pte(0, PTE1, 0x0, 0, 256);

  /* 0xc0000000 ~ 0xc00f0000 for kernel (physical 0x0 ~ 0xf0000), 960K. */
  setup_pte(768, PTE2, 0x0, 0, 240);

  /* leave a memory hole here as guard pages. */

  /* 0xc0400000 ~ 0xc0c00000 for memory pool (physical 0x100000 ~ 0x900000), 8M. */
  setup_pte(769, PTE3, 0x100000, 0, 1024);
  setup_pte(770, PTE4, 0x500000, 0, 1024);

  /* reload the new page directory. */
  __asm__ __volatile__ ("movl %0, %%cr3" : : "r" (pde_addr));
}

} /* mm */
