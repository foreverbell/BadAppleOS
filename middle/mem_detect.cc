
__asm__ (".code16gcc\n");

#include "../kernel/include/stdint.h"
#include "../kernel/include/linkage.h"

/*
 * Structure of an entry (24 bytes long)
 * first qword: base address;
 * second qword: length of region;
 * next dword: region type;
 * next dword: ACPI 3.0 Extended Attributes bitfield (optional).
 */

asmlinkage int mem_detect_cc(uint8_t *buffer, uint32_t max_entries) {
	const uint32_t magic_number = 0x534D4150;
	
	uint32_t flags = 0, entries = 0;
	int signature, read_bytes;
	uint32_t acpi, length_low, length_high;
	bool ignore;

	do {
		__asm__ __volatile__ ("int $0x15"
		  : "=a"(signature), "=c"(read_bytes), "=b"(flags)
		  : "a"(0xE820), "b"(flags), "c"(24), "d"(magic_number), "D"(buffer));
		
		if (signature != magic_number) {
			return -1;
		}
	
		ignore = false;

		length_low = *(uint32_t *)(buffer + 8);
		length_high = *(uint32_t *)(buffer + 12);

		if (length_low == 0 && length_high == 0) {
			ignore = true;
		}

		if (read_bytes > 20) {
			acpi = *(uint32_t *)(buffer + 20);
			if ((acpi & 1) == 0) {
				ignore = true;
			}
		}

		if (!ignore) {
			buffer += 24;
			entries += 1;
		}
	} while (flags != 0 && entries < max_entries);

	return entries;
}
