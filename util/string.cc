
#include <string.h>

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *) s;
    while (n--) {
    	*p++ = (uint8_t) c;
	}
    return s;
}

void *memcpy(void *d, void *s, size_t n) {
	__asm__ ("cld\n\t"
			 "rep\n\t"
			 "movsb"
			 ::"c"(n), "S"(s),"D"(d));
	return d;
}