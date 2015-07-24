
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <algorithm>

using std::sort;

namespace mm {
	
namespace {

#pragma pack(push, 1)
struct smap_entry_t {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t acpi;
} __attribute__((packed));
#pragma pack(pop)

int entries_count;
smap_entry_t *entries;

}

void detect(void) {
	const char *spearate_l1 = "+------------+------------+------+------+";
	const char *spearate_l2 = "+============+============+======+======+";
	
	entries_count = (int) *(uint32_t *) 0x8000;
	entries = (smap_entry_t *) 0x8004;

	printf("Total memory entries = %d.\n", entries_count);
	
	/* std::sort doesn't rely on memory allocation. So it is guaranteed to be safe. */ 
	sort(entries, entries + entries_count, [&](const smap_entry_t &a, const smap_entry_t &b) -> bool {
		return a.base < b.base;
	});
	
	int sum = 0;
	
	puts(spearate_l1);
	puts("|    base    |   length   | type | free |");
	puts(spearate_l2);
	
	for (int i = 0; i < entries_count; ++i) {
		bool free = (entries[i].type == 1);
		
		if (free) {
			sum += entries[i].length;
		}
		printf("| 0x%08x | 0x%08x |   %d  |   %c  |\n", 
			(int) entries[i].base, (int) entries[i].length, entries[i].type, free ? 'Y' : 'N');
	}
	puts(spearate_l1);
	printf("Total free memory = %dM.\n", sum / 1024 / 1024); 
}

} /* mm */ 
