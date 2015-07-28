#include <system.h>
#include <memory.h>
#include <abi.h>
#include <cpu_manipulate.h>
#include <console.h>
#include <timer.h>
#include <stdio.h>
#include <linkage.h>

extern void play(void);
 
asmlinkage void kinitialize(void) {	
	/* no cursor, and no blink text. */
	console::initialize(false, false);
	puts("Successfully landed to protected mode.");

	cpu::initialize();
	gdt::initialize();
	mm::initialize();
	idt::initialize();
	isr::initialize();
	irq::initialize();
	timer::initialize();
	abi::ctors();

	/* enable interrupt. */
	cpu::sti(); 

	play();
}
