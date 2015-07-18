#include <system.h>
#include <memory.h>
#include <console.h>
#include <stdio.h>
#include <linkage.h>

extern void play(void);
extern void __ctor(void);

asmlinkage void kernel_init(void) {
	
	console::initialize(false); // We don't need cursor anyway
	printf("Successfully landed to protected mode.\n");
	
	gdt::initialize();
	idt::initialize();
	isr::initialize();
	irq::initialize();
	timer::initialize();
	memory::detect();

	__ctor();

	__asm__ __volatile__ ("sti");
	
	play();
}
