
#include <string.h>

void *memset(void *s, int v, size_t n) {
  __asm__ __volatile__ (
    "cld\n\t"
    "rep\n\t"
    "stosb"
    : /* no outputs */
    : "a"(v), "D"(s), "c"(n));
  return s;
}

void *memcpy(void *d, const void *s, size_t n) {
  __asm__ __volatile__ (
    "cld\n\t"
    "rep\n\t"
    "movsb"
    : /* no outputs */
    : "c"(n), "S"(s), "D"(d));
  return d;
}
