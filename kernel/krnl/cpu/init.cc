#include <cpu.h>
#include <stdio.h>

namespace cpu {

void initialize(void) {
	static char vendor_buffer[16];

	if (cpuid::support()) {
		cpuid::vendor(vendor_buffer);
		printf("CPUID vendor = %s.\n", vendor_buffer);
	}
}

} /* cpu */