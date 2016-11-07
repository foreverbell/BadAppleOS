
#include <cpu.h>
#include <assert.h>

namespace cpu {

namespace cpuid {

/* because gcc doesn't support naked function, so the only way is to write shell code. */
/*
  pushfd
  pushfd
  xor dword [esp], 0x00200000
  popfd
  pushfd
  pop eax
  xor eax, [esp]
  popfd
  and eax, 0x00200000
  ret
*/

const char *shell_code = "\x66\x9C\x66\x9C\x66\x67\x81\x34\x24\x00\x00\x20\x00\x66\x9D\x66\x9C\x66\x58\x66\x67\x33\x04\x24\x66\x9D\x66\x25\x00\x00\x20\x00\xC3";

bool support() {
  int ret = 0;
  __asm__ __volatile__ (
    "call %%eax"
    : "=a"(ret)
    : "a"(shell_code)
  );
  return ret ? true : false;
}

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
