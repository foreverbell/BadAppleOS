#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

namespace port {
  
#define PORT_PIT_CHANNEL0  0x40
#define PORT_PIT_COMMAND   0x43

#define PORT_CMOS_COMMAND  0x70
#define PORT_CMOS_DATA     0x71

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0":"=a"(ret):"dN"(port));
    return ret;
}

inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__ ("inw %1, %0":"=a"(ret):"dN"(port));
    return ret;
}

inline void outb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("outb %1, %0"::"dN"(port),"a"(data));
}

inline void outw(uint16_t port, uint16_t data) {
    __asm__ __volatile__ ("outw %1, %0"::"dN"(port),"a"(data));
#ifdef FORCE_IO_WAIT
	/* Forces the CPU to wait for an I/O operation to complete. */
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
#endif
}

} /* port */

namespace cmos {
    
inline uint8_t read(uint8_t which) {
#ifdef NMI_DISABLE
	which |= (1 << 7);
#endif
	port::outb(PORT_CMOS_COMMAND, which);
	return port::inb(PORT_CMOS_DATA);
}

} /* cmos */

#endif
