
#include <system.h>
#include <stdint.h>
#include <string.h>

namespace gdt {

namespace {

#define MAX_GDT_ENTRY 256

#pragma pack(push, 1)
struct gdt_entry_t {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_descriptior_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
#pragma pack(pop)

gdt_entry_t gdt[MAX_GDT_ENTRY];
gdt_descriptior_t gdt_ptr;

void flush(void) {
	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.base = (uint32_t) gdt;

	__asm__ __volatile__ (
		"lgdt %0\n\t"
		"ljmp $0x8, $1f\n\t" /* far jump to the code segment. */
		"1:\n\t"
		"movw $0x10, %%ax\n\t" /* set data segment. */
		"movw %%ax, %%ds\n\t"
		"movw %%ax, %%ss\n\t"
		"movw %%ax, %%es\n\t"
		"movw %%ax, %%fs\n\t"
		"movw %%ax, %%gs\n\t"
		:"=m"(gdt_ptr)
	);
}

}

void set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	gdt[index].base_low = base & 0xffff;
	gdt[index].base_middle = (base >> 16) & 0xff;
	gdt[index].base_high = (base >> 24) & 0xff;
	gdt[index].limit = limit;
	gdt[index].access = access;
	gdt[index].granularity = granularity;
}

void initialize(void) {
	/* initialize all gdt entries. */
	memset(gdt, 0, sizeof(gdt));

	/* initialize gdt ptr. */
	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.base = (uint32_t) gdt;

	/* setup gdt gates. */
	set_gate(0, 0, 0x0, 0x0, 0x0); // null gdt entry
	set_gate(1, 0, 0xffff, 0x9a, 0xcf); // code segment
	set_gate(2, 0, 0xffff, 0x92, 0xcf); // data segment

	/* flush gdt */
	flush();
}

} /* gdt */ 
