#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

namespace port {

#define PORT_MASTER_PIC_CMD 0x20
#define PORT_MASTER_PIC_DAT 0x21
#define PORT_SLAVE_PIC_CMD  0xa0
#define PORT_SLAVE_PIC_DAT  0xa1

#define PORT_PIT_CHANNEL0   0x40
#define PORT_PIT_CHANNEL1   0x41
#define PORT_PIT_CHANNEL2   0x42
#define PORT_PIT_CMD        0x43

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
  
inline uint8_t read(uint8_t which) {
#ifdef NMI_DISABLE
  which |= (1 << 7);
#endif
  port::outb(PORT_CMOS_CMD, which);
  return port::inb(PORT_CMOS_DAT);
}

} /* cmos */

#endif
