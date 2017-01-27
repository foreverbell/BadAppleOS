
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <port.h>

namespace mm {

/*
 * To be simple, we reuse physical memory of [0x1000, 0x6000) for PDE and PTE.
 * Physical memory layout of PDE && PTE.
 *   [0x1000, 0x2000) PDE
 *   [0x2000, 0x3000) PTE1
 *   [0x3000, 0x4000) PTE2
 *   [0x4000, 0x5000) PTE3
 *   [0x5000, 0x6000) PTE4
 */

#define PAGE_PRESENT 1
#define PAGE_RW      2

namespace {

uint32_t * const pde_addr = (uint32_t *) 0x1000;

/* mapping (base_addr, base_addr+to*4K) to index*4M. */
void initialize_pte(int index, uint32_t pte_addr, int from, int to, uint32_t base_addr) {
  pde_addr[index] = pte_addr | PAGE_PRESENT | PAGE_RW;
  uint32_t *p = (uint32_t *) pte_addr;

  for (int i = 0; i < 1024; ++i) {
    p[i] = 0;
    if (i >= from && i < to) {
      p[i] = ((i << 12) + base_addr) | PAGE_PRESENT | PAGE_RW;
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

  printf("Physical memory: %uK available, base = %uK, extended = %uK\n", totalmem, basemem, totalmem - basemem);

  /* reinitialize PDE and PTE. */

  /* clear all PDE entries. */
  for (int i = 0; i < 1024; ++i) {
    pde_addr[i] = 0;
  }

  /* set lower 1M memory as identity paging. */
  initialize_pte(0, 0x2000, 0, 256, 0);

  /* 0xC0000000 ~ 0xC0080000 for kernel (physical 0x10000 ~ 0x90000), 512K. */
  initialize_pte(768, 0x3000, 0, 128, 0x10000);

  /* 0xC0400000 ~ 0xC0C00000 for memory pool (physical 0x100000 ~ 0x900000), 8M, two entries. */
  initialize_pte(769, 0x4000, 0, 1024, 0x100000);
  initialize_pte(770, 0x5000, 0, 1024, 0x500000);

  /* reload the new page directory. */
  __asm__ __volatile__ ("movl %0, %%cr3" : : "r" (pde_addr));
}

} /* mm */
