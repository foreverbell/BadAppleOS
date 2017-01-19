#include <system.h>
#include <memory.h>
#include <abi.h>
#include <cpu.h>
#include <console.h>
#include <timer.h>
#include <stdio.h>
#include <linkage.h>

extern void play(void);

namespace {

void idle(void) {
  while (true) {
    cpu::manipulate::halt();
  }
}

}

asmlinkage void kinitialize(void) {
  /* console: no cursor, and no blink text. */
  console::initialize(false, false);
  puts("Successfully landed to protected mode.");

  gdt::initialize();
  mm::initialize();
  idt::initialize();
  isr::initialize();
  irq::initialize();
  timer::initialize();

  /* call all C++ constructors. */
  abi::ctors();

  /* enable interrupt. */
  cpu::manipulate::sti();

  /* play our BadApple animation. */
  play();

  /* execute idle process. */
  idle();
}
