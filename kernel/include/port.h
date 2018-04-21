#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

namespace port {

#define PORT_CMOS_CMD       0x70
#define PORT_CMOS_DAT       0x71

inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ __volatile__ (
        "inb %1, %0"
    : "=a"(ret)
    : "dN"(port)
  );
  return ret;
}

inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  __asm__ __volatile__ (
    "inw %1, %0"
    : "=a"(ret)
    : "dN"(port)
  );
  return ret;
}

inline void outb(uint16_t port, uint8_t data) {
  __asm__ __volatile__ (
    "outb %1, %0"
    :
    : "dN"(port), "a"(data)
  );
}

inline void outw(uint16_t port, uint16_t data) {
  __asm__ __volatile__ (
    "outw %1, %0"
    :
    : "dN"(port),"a"(data)
  );
}

inline void wait(void) {
  __asm__ __volatile__ (
    "outb %%al, $0x80" 
    : 
    : "a"(0)
  ); 
}

} /* port */

namespace cmos {
  
inline uint8_t read(uint8_t r) {
#ifdef NMI_DISABLE
  r |= (1 << 7);
#endif
  port::outb(PORT_CMOS_CMD, r);
  return port::inb(PORT_CMOS_DAT);
}

} /* cmos */

#endif
