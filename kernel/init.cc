#include <system.h>
#include <memory.h>
#include <console.h>
#include <stdio.h>
#include <linkage.h>
#include <assert.h>

extern void play(void);

asmlinkage void kinitialize(void) {
	
	/* we don't need cursor anyway. */
	console::initialize(false);
	printf("Successfully landed to protected mode.\n");
	
	cpu::initialize();
	gdt::initialize();
	idt::initialize();
	isr::initialize();
	irq::initialize();
	timer::initialize();
	mm::detect();
	ctor::initialize();

	/* enable interrupt. */
	cpu::sti(); 
	
	play();
}
