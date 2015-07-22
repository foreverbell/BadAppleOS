
#include <malloc.h>
#include <panic.h>
#include <stdio.h>

void *operator new(size_t n) {
	void *p = malloc(n);
	printf("[new] %d bytes, pointer = 0x%x\n", n, (int) p);
	if (p == NULL) {
		panic::panic("Bad alloc.");
	}
	return p;
}

void operator delete(void *p) {
	printf("[delete] pointer = 0x%x\n", (int) p);
	return free(p);
}

void *operator new [](size_t n) {
	void *p = malloc(n);
	printf("[new] %d bytes, pointer = 0x%x\n", n, (int) p);
	if (p == NULL) {
		panic::panic("Bad alloc.");
	}
	return p;
}

void operator delete [](void *p) {
	printf("[delete] pointer = 0x%x\n", (int) p);
	return free(p);
}
