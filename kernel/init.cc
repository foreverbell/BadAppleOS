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
  console::initialize(false);
  puts("Successfully landed to protected mode.");

  mm::initialize();
  gdt::initialize();
  idt::initialize();
  isr::initialize();
  irq::initialize();
  timer::initialize();

  /* enable interrupt. */
  cpu::manipulate::sti();

  /* call all C++ constructors. */
  abi::ctors();

  /* play our BadApple animation. */
  play();

  /* execute idle process. */
  idle();
}

