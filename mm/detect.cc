
#include <stdint.h>
#include <stdio.h>
#include <memory.h>

namespace memory {
	
#pragma pack(push, 1)
struct smap_entry_t {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t acpi;
} __attribute__((packed));
#pragma pack(pop)

static int entries_count;
static smap_entry_t *entries;

void detect(void) {
	entries_count = (int) *(uint32_t *)0x8000;
	entries = (smap_entry_t *)0x8004;

	printf("entries = %d\n", entries_count);
	
	int sum = 0;
	
	for (int i = 0; i < entries_count; ++i) {
		printf("base = %x, length = %x, type = %d, acpi = %d\n", 
			(int) entries[i].base, (int) entries[i].length, entries[i].type, entries[i].acpi);
		sum += entries[i].length;
	}
	printf("total memory = %dMB\n", sum / 1024 / 1024); 
}


} /* memory */ 
