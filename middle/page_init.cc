
__asm__ (".code16gcc\n");

#include "../kernel/include/stdint.h"
#include "../kernel/include/linkage.h"

/*
 * memory layout of PDE && PTE
 * 0x1000 ~ 0x1FFF PDE
 * 0x2000 ~ 0x2FFF PTE1
 * 0x3000 ~ 0x3FFF PTE2
 * 0x4000 ~ 0x4FFF PTE3
 * 0x5000 ~ 0x5FFF PTE4
 */

/* aligned by 4K. */
uint32_t * const pde_addr = (uint32_t *) 0x1000;
const uint32_t pte1_addr = 0x2000;
const uint32_t pte2_addr = 0x3000;
const uint32_t pte3_addr = 0x4000;
const uint32_t pte4_addr = 0x5000;

const int page_present = 1;
const int page_rw = 2;

/* mapping (base_addr, base_addr+to*4K) to index*4M. */
void init_pte(int index, uint32_t pte_addr, int to, uint32_t base_addr) {
	pde_addr[index] = pte_addr | page_present | page_rw;
	uint32_t *p = (uint32_t *) pte_addr;
	
	for (int i = 0; i < 1024; ++i) {
		p[i] = i < to ? ((i << 12) + base_addr) | page_present | page_rw : 0;
	}
}

asmlinkage void init_paging() {
	/* clear all PDE entries. */
	for (int i = 0; i < 1024; ++i) {
		pde_addr[i] = 0;
	}
	
	/* set lower 1M memory as identity paging. */
	init_pte(0, pte1_addr, 256, 0);
	
	/* 0xC0000000 ~ 0xC0080000 for kernel (physical 0x10000 ~ 0x90000), 512K. */
	init_pte(768, pte2_addr, 128, 0x10000);
	
	/* 0xC0400000 ~ 0xC0C00000 for memory pool (physical 0x100000 ~ 0x900000), 8M, two entries. */
	init_pte(769, pte3_addr, 1024, 0x100000);
	init_pte(770, pte4_addr, 1024, 0x500000);
}
