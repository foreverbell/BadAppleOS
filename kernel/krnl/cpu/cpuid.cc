
#include <cpu.h>
#include <assert.h>

namespace cpu {
	
namespace cpuid {

/* issue a cpuid request. input: eax; output: eax, ebx, ecx, edx. */
void request(int *eax, int *ebx, int *ecx, int *edx) {
	int code = *eax, dummy;
	
#define CHECK(r) \
	if ((r) == 0) { \
		(r) = &dummy; \
	}
	CHECK(ebx);
	CHECK(ecx);
	CHECK(edx);
#undef CHECK
	
	/* only function 0/1 supported. */
	assert(code == 0 || code == 1);
	__asm__ __volatile__ (
		"cpuid"
		: "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
		: "a"(code)
	);
}

void vendor(char *buffer) {
	int eax = 0;
	
	request(&eax, (int *) (buffer + 0), (int *) (buffer + 8), (int *) (buffer + 4));
}

void feature(int *ecx, int *edx) {
	int eax = 1;
	
	request(&eax, NULL, ecx, edx);
}

} /* cpuid */

} /* cpu */
