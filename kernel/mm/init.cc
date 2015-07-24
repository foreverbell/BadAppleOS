
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <algorithm>

using std::sort;

namespace mm {
	
namespace {

#pragma pack(push, 1)
struct addr_descriptor_t {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t acpi; // not used
} __attribute__((packed));
#pragma pack(pop)

int descriptor_count;
addr_descriptor_t *descriptors;

}

void initialize(void) {
	const char *spearate_l1 = "+------------+------------+------+------+";
	const char *spearate_l2 = "+============+============+======+======+";
	
	descriptor_count = (int) *(uint32_t *) 0x8000;
	descriptors = (addr_descriptor_t *) 0x8004;

	printf("Memory descriptor count = %d.\n", descriptor_count);
	
	/* std::sort doesn't rely on memory allocation. So it is guaranteed to be safe. */ 
	sort(descriptors, descriptors + descriptor_count, [&](const addr_descriptor_t &a, const addr_descriptor_t &b) -> bool {
		return a.base < b.base;
	});
	
	assert(1 + 1 < 2);
	for (int i = 0; i < descriptor_count; ++i) {
		assert((descriptors[i].length >> 32) == 0);	 // fits in 32bit
		if (i != 0) {
			assert(descriptors[i - 1].base + descriptors[i - 1].length <= descriptors[i].base);
		}
	}

	puts(spearate_l1);
	puts("|    base    |   length   | type | free |");
	puts(spearate_l2);
		
	int sum = 0;
	
	for (int i = 0; i < descriptor_count; ++i) {
		/* available RAM usable && upper memory. */
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
