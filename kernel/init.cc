#include <system.h>
#include <memory.h>
#include <console.h>
#include <stdio.h>
#include <linkage.h>
#include <assert.h>

extern void play(void);

asmlinkage void kinitialize(void) {
	
	console::initialize(false); // We don't need cursor anyway
	printf("Successfully landed to protected mode.\n");
	
	gdt::initialize();
	idt::initialize();
	isr::initialize();
	irq::initialize();
	timer::initialize();
	mm::detect();
	ctor::initialize();

	__asm__ __volatile__ ("sti");

	play();
}
