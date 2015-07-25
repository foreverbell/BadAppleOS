
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <algorithm>

using std::sort;

namespace mm {
	
namespace {

#pragma pack(push, 1)
struct mm_descriptor_t {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t acpi; // not used
} __attribute__((packed));
#pragma pack(pop)

int descriptor_count;
mm_descriptor_t *descriptors;

}

void initialize(void) {
	const char *spearate_l1 = "+------------+------------+------+------+";
	const char *spearate_l2 = "+============+============+======+======+";
	
	descriptor_count = (int) *(uint32_t *) 0x8000;
	descriptors = (mm_descriptor_t *) 0x8004;

	printf("Memory descriptor count = %d.\n", descriptor_count);
	
	/* std::sort doesn't rely on memory allocation. So it is guaranteed to be safe. */ 
	sort(descriptors, descriptors + descriptor_count, [&](const mm_descriptor_t &a, const mm_descriptor_t &b) -> bool {
		return a.base < b.base;
	});
	
	/* aligned by page size. */
	for (int i = 0; i < descriptor_count; ++i) {
		uint64_t base = descriptors[i].base;
		uint64_t length = descriptors[i].length;
		
		if (base & 0xfff) {
			base = (base & ~0xfffll) + 0x1000;
		}
		if (length & 0xfff) {
			length = length & ~0xfffll;
		}
		descriptors[i].base = base;
		descriptors[i].length = length;
	}
	 
	/* check if all lengths are below 4G, and memory ranges not overlap. */
	for (int i = 0; i < descriptor_count; ++i) {
		assert((descriptors[i].length >> 32) == 0);	
		if (i != 0) {
			assert(descriptors[i - 1].base + descriptors[i - 1].length <= descriptors[i].base);
		}
	}

	puts(spearate_l1);
	puts("|    base    |   length   | type | free |");
	puts(spearate_l2);
		
	int sum = 0;
	
	for (int i = 0; i < descriptor_count; ++i) {
		/* free iff type is available RAM usable && upper memory. */
		bool free = (descriptors[i].type == 1 && descriptors[i].base >= 0x100000);
		
		if (free) {
			sum += descriptors[i].length;
		}
		printf("| 0x%08x | 0x%08x |   %d  |   %c  |\n", 
			(int) descriptors[i].base, (int) descriptors[i].length, descriptors[i].type, free ? 'Y' : 'N');
	}
	puts(spearate_l1);
	printf("Total free memory = %dM.\n", sum / 1024 / 1024); 
}

} /* mm */ 
