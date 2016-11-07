[extern _irq_dispatcher]

%macro irq 1
  global _irq_handler%1
  _irq_handler%1:
    cli
    push %1     ; irq index
    jmp irq_routine
%endmacro

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15

irq_routine:
  pushad
  push ds
  push es
  push fs
  push gs
  mov eax, 0x10  ; load kernel data segment
  mov ds, eax    ; load to the segment registers
  mov es, eax
  mov fs, eax
  mov gs, eax
  push esp       ; pointer to all stuff we have pushed, passed to irq_dispatcher
  call _irq_dispatcher
  pop esp
  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 0x4   ; pop irq index
  iret
