
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <port.h>

namespace mm {

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

/* map VA `base+4K*[from, to)` to PA `index*4M+4K*[from, to)`. */
void initialize_pte(int index, uint32_t pte_addr,
                    int from, int to, uint32_t base) {
  pde_addr[index] = pte_addr | PAGE_PRESENT | PAGE_WRITE;
  uint32_t *p = (uint32_t *) pte_addr;

  for (int i = 0; i < 1024; ++i) {
    p[i] = 0;
    if (i >= from && i < to) {
      p[i] = ((i << 12) + base) | PAGE_PRESENT | PAGE_WRITE;
    }
  }
}

}

void initialize(void) {
  size_t basemem, extmem, ext16mem, totalmem;

  /* use CMOS calls to measure available base & extended memory. */
  /* measured in kilobytes. */
  basemem = cmos::read(PORT_NVRAM_BASELO) + (cmos::read(PORT_NVRAM_BASEHI) << 8);
  extmem = cmos::read(PORT_NVRAM_EXTLO) | (cmos::read(PORT_NVRAM_EXTHI) << 8);
  ext16mem = (cmos::read(PORT_NVRAM_EXT16LO) | (cmos::read(PORT_NVRAM_EXT16HI) << 8)) << 6;

  if (ext16mem) {
    totalmem = 16 * 1024 + ext16mem;
  } else if (extmem) {
    totalmem = 1 * 1024 + extmem;
  } else {
    totalmem = basemem;
  }

  printf("Physical memory: %uK available, base = %uK, extended = %uK\n",
         totalmem, basemem, totalmem - basemem);

  /* reinitialize PDE and PTE. */

  /* clear all PDE entries. */
  for (int i = 0; i < 1024; ++i) {
    pde_addr[i] = 0;
  }

  /* set lower 1M memory as identity paging. */
  initialize_pte(0, PTE1, 0, 256, 0);

  /* 0xc0000000 ~ 0xc00f0000 for kernel (physical 0x0 ~ 0xf0000), 960K. */
  initialize_pte(768, PTE2, 0, 240, 0x0);

  /* leave 128K memory here as guard pages. */

  /* 0xc0400000 ~ 0xc0c00000 for memory pool (physical 0x100000 ~ 0x900000), 8M. */
  initialize_pte(769, PTE3, 0, 1024, 0x100000);
  initialize_pte(770, PTE4, 0, 1024, 0x500000);

  /* reload the new page directory. */
  __asm__ __volatile__ ("movl %0, %%cr3" : : "r" (pde_addr));
}

} /* mm */
