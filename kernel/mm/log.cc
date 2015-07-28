
#include <stdio.h>
#include <stdint.h>

namespace mm {

namespace {

int silent = 0;

}

void log_status(int to) {
	silent = to;
}

void log_new(size_t n, void *p) {
	if (!silent) {
		printf("[new] %u bytes, pointer = 0x%x.\n", n, p);
	}
}

void log_delete(void *p) {
	if (!silent) {
		printf("[delete] pointer = 0x%x.\n", p);
	}
}

} /* mm */