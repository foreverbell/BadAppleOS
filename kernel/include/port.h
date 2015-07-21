#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

inline uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0":"=a"(ret):"dN"(port));
    return ret;
}

inline uint16_t inportw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__ ("inw %1, %0":"=a"(ret):"dN"(port));
    return ret;
}

inline void outportb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("outb %1, %0"::"dN"(port),"a"(data));
}

inline void outportw(uint16_t port, uint16_t data) {
    __asm__ __volatile__ ("outw %1, %0"::"dN"(port),"a"(data));
#ifdef FORCE_IO_WAIT
	/* Forces the CPU to wait for an I/O operation to complete. */
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0));
#endif
}

#endif
