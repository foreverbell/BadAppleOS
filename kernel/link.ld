phys = 0x10000;    /* physical address of entry point. */

STARTUP(begin.o)

SECTIONS
{
  . = 0xc0000000;

  /* AT(phys) for load memory address of this section. */
  .mbheader ALIGN(4) : AT(phys) {
    mbheader = .;
    *(.mbheader)
  }

  .text ALIGN(4) : AT(phys + (code - mbheader)) {
    code = .;
    *(.text .text.*)
    *(.rodata .rodata.*)
  }

  .data ALIGN(4) : AT(phys + (data - mbheader)) {
    data = .;
    *(.data)
  }

  .ctors ALIGN(4) : AT(phys + (ctors - mbheader)) {
    ctors = .;
    ___CTOR_LIST__ = .;
    LONG((___CTOR_END__ - ___CTOR_LIST__) / 4 - 2)
    *(.ctors)
    LONG(0)
    ___CTOR_END__ = .;

    ___INIT_ARRAY_LIST__ = .;
    LONG((___INIT_ARRAY_END__ - ___INIT_ARRAY_LIST__) / 4 - 2)
    *(.init_array)
    LONG(0)
    ___INIT_ARRAY_END__ = .;

    /* dtor omitted. */
  }

  .bss ALIGN(4) : AT(phys + (bss - mbheader)) {
    bss = .;
    *(.bss)
  }

  /DISCARD/ : {
    *(.eh_frame .note.GNU-stack)
  }

  end = .;
}
