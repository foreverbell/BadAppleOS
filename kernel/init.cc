#include <system.h>
#include <memory.h>
#include <abi.h>
#include <console.h>
#include <stdio.h>
#include <linkage.h>

extern void play(void);

asmlinkage void kinitialize(void) {
	
	/* we don't need cursor anyway. */
	console::initialize(false);
	puts("Successfully landed to protected mode.");
	
	cpu::initialize();
	gdt::initialize();
	idt::initialize();
	isr::initialize();
	irq::initialize();
	timer::initialize();
	mm::detect();
	abi::ctors();

	/* enable interrupt. */
	cpu::sti(); 

	play();
}
