
#include <stdlib.h>

static unsigned long int next = 1;

int rand(void) {
  next = next * 214013L + 2531011L;
  return (unsigned int) (next >> 16) & 0x7fff;
}

void srand(unsigned int seed) {
  next = seed;
}
