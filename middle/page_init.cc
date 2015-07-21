
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
const void *pde_address  = (void *) 0x1000;
const void *pte1_address = (void *) 0x2000;
const void *pte2_address = (void *) 0x3000;
const void *pte3_address = (void *) 0x4000;
const void *pte4_address = (void *) 0x5000;

const int page_present = 1;
const int page_rw = 2;

asmlinkage void init_paging() {
	uint32_t *p, *pde_entry = (uint32_t *) pde_address;
	
	/* clear all PDE entries. */
	for (int i = 0; i < 1024; ++i) {
		pde_entry[i] = 0;
	}
	
	/* initialize the required PDE entries. */
	pde_entry[0] = (uint32_t) pte1_address | page_present | page_rw;
	pde_entry[768] = (uint32_t) pte2_address | page_present | page_rw;
	pde_entry[769] = (uint32_t) pte3_address | page_present | page_rw;
	pde_entry[770] = (uint32_t) pte4_address | page_present | page_rw;
		
	/* set lower 1M memory as identity paging. */
	p = (uint32_t *) pte1_address;
	for (int i = 0; i < 1024; ++i) {
		p[i] = i < 256 ? (i << 12) | page_present | page_rw : 0;
	}
	
	/* 0xC0000000 ~ 0xC0080000 for kernel (physical 0x10000 ~ 0x90000), 512K. */
	p = (uint32_t *) pte2_address;
	for (int i = 0; i < 1024; ++i) {
		p[i] = i < 128 ? ((i << 12) + 0x10000) | page_present | page_rw : 0;
	}
	
	/* 0xC0400000 ~ 0xC0C00000 for memory pool (physical 0x100000 ~ 0x900000), 8M, two entries. */
	p = (uint32_t *) pte3_address;
	for (int i = 0; i < 1024; ++i) {
		p[i] = ((i << 12) + 0x100000) | page_present | page_rw;
	}
	
	p = (uint32_t *) pte4_address;
	for (int i = 0; i < 1024; ++i) {
		p[i] = ((i << 12) + 0x500000) | page_present | page_rw;
	}
}
