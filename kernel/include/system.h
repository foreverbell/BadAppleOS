#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

namespace power {
	
void reboot(void);

} /* power */

namespace cpu {
	
void initialize(void);
void cli(void);
void sti(void);
void halt(void);
	
} /* cpu */

namespace gdt {

void initialize(void);
void set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);

} /* gdt */

namespace idt {

void initialize(void);
void set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

} /* idt */

namespace isr {

void initialize(void);

} /* isr */ 

namespace irq {

#pragma pack(push, 1)
struct irq_context_t {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t irq_index;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed));
#pragma pack(pop)

typedef void (* fn_irq_handler_t)(irq_context_t *);

void initialize(void);
void install(int, fn_irq_handler_t);
void uninstall(int);
void enable(int index);
void disable(int index);

} /* irq */ 

namespace timer {

typedef void (* fn_timer_callback_t)(uint64_t, int);

void initialize(void);
uint64_t get_system_tick(void);
int add(fn_timer_callback_t);
bool remove(int);

} /* timer */

namespace ctor {

void initialize(void);

} /* ctor */

#endif