
#include <stdint.h>
#include <system.h>
#include <stdio.h>
#include <string.h>
#include <linkage.h>


asmlinkage {
void irq_handler0();
void irq_handler1();
void irq_handler2();
void irq_handler3();
void irq_handler4();
void irq_handler5();
void irq_handler6();
void irq_handler7();
void irq_handler8();
void irq_handler9();
void irq_handler10();
void irq_handler11();
void irq_handler12();
void irq_handler13();
void irq_handler14();
void irq_handler15();
}

namespace irq {
	
const int irq_vector_offset = 32;
const int max_irq_entry = 16;

static fn_irq_handler_t lpfn_irq_handler[max_irq_entry];

void initialize(void) {
	/* remap IRQ to proper IDT entries (32 ~ 47) */
	outportb(0x20, 0x11);
	outportb(0xa0, 0x11);
	outportb(0x21, irq_vector_offset);     // vector offset for master PIC is 32
	outportb(0xa1, irq_vector_offset + 8); // vector offset for slave PIC is 40
	outportb(0x21, 0x4);                   // tell master PIC that there is a slave PIC at IRQ2
	outportb(0xa1, 0x2);                   // tell slave PIC its cascade identity
	outportb(0x21, 0x1);
	outportb(0xa1, 0x1);
	/* disable all IRQs by default. */
	outportb(0x21, 0xff);
	outportb(0xa1, 0xff);

	/* initialize ISR to correct entries in the IDT.*/
#define set_irq(n) idt::set_gate(n + irq_vector_offset, (uint32_t) irq_handler##n, 0x8, 0x8e);
	set_irq(0);
	set_irq(1);
	set_irq(2);
	set_irq(3);
	set_irq(4);
	set_irq(5);
	set_irq(6);
	set_irq(7);
	set_irq(8);
	set_irq(9);
	set_irq(10);
	set_irq(11);
	set_irq(12);
	set_irq(13);
	set_irq(14);
	set_irq(15);
#undef set_irq

	memset(lpfn_irq_handler, 0, sizeof(lpfn_irq_handler));
}

/* install an IRQ handler. */
void install(int index, fn_irq_handler_t handler) {
	if (lpfn_irq_handler[index] != NULL) {
		printf("[irq::install] IRQ%d handler already exists, overwritten.\n", index);	
	}
	lpfn_irq_handler[index] = handler;
}

/* uninstall an IRQ handler. */
void uninstall(int index) {
	if (lpfn_irq_handler[index] == NULL) {
		printf("[irq::uninstall] IRQ%d handler not exists.\n", index);
	}
	lpfn_irq_handler[index] = NULL;
}

/* disable an IRQ by setting mask. */
void disable(int index) {
	uint16_t port;
	uint8_t value;

	if (index < 8) {
		port = 0x21;
	} else {
		port = 0xa1;
		index -= 8;
	}
	value = inportb(port) | (1 << index);
	outportb(port, value);
}

/* enable an IRQ by clearing mask. */
void enable(int index) {
	uint16_t port;
	uint8_t value;

	if (index < 8) {
		port = 0x21;
	} else {
		port = 0xa1;
		index -= 8;
	}
	value = inportb(port) & ~(1 << index);
	outportb(port, value);
}

static void dispatcher(irq_context_t *ptr) {
	int irq_index = ptr->irq_index;
	fn_irq_handler_t handler = lpfn_irq_handler[irq_index];

	if (handler == NULL) {
		printf("[IRQ dispatcher] Unhandled IRQ %d.\n", irq_index);
	} else {
		handler(ptr);
	}

	/* send an EOI (end of interrupt) to indicate that we are done. */
	if (irq_index >= 8) {
		outportb(0xa0, 0x20);
	}
	outportb(0x20, 0x20);
}

} /* irq */ 

asmlinkage void irq_dispatcher(irq::irq_context_t *ptr) {
	irq::dispatcher(ptr);
}
