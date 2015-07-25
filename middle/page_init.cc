
__asm__ (".code16gcc\n");

#include "../kernel/include/stdint.h"
#include "../kernel/include/linkage.h"

/*
 * physical memory layout of PDE && PTE
 * 0x1000 ~ 0x1FFF PDE
 * 0x2000 ~ 0x2FFF PTE1
 * 0x3000 ~ 0x3FFF PTE2
 * 0x4000 ~ 0x4FFF PTE3
 * 0x5000 ~ 0x5FFF PTE4
 */

namespace {

/* aligned by 4K. */
uint32_t * const pde_addr = (uint32_t *) 0x1000;

#define PAGE_PRESENT 1
#define PAGE_RW      2

/* mapping (base_addr, base_addr+to*4K) to index*4M. */
void init_pte(int index, uint32_t pte_addr, int from, int to, uint32_t base_addr) {
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

asmlinkage void init_paging() {
	/* clear all PDE entries. */
	for (int i = 0; i < 1024; ++i) {
		pde_addr[i] = 0;
	}
	
	/* set lower 1M memory as identity paging. */
	init_pte(0, 0x2000, 0, 256, 0);
	
	/* 0xC0000000 ~ 0xC0080000 for kernel (physical 0x10000 ~ 0x90000), 512K. */
	init_pte(768, 0x3000, 0, 128, 0x10000);
	
	/* 0xC0400000 ~ 0xC0C00000 for memory pool (physical 0x100000 ~ 0x900000), 8M, two entries. */
	init_pte(769, 0x4000, 0, 1024, 0x100000);
	init_pte(770, 0x5000, 0, 1024, 0x500000);
}
