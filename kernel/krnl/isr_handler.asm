[extern _isr_dispatcher]

%macro isr 1
  global _isr_handler%1
  _isr_handler%1:
    cli
    push 0      ; dummy error code
    push %1     ; exception index
    jmp isr_routine
%endmacro

%macro isr_ec 1     ; with error code
  global _isr_handler%1
  _isr_handler%1:
    cli
    push %1
    jmp isr_routine
%endmacro

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_ec 8
isr 9
isr_ec 10
isr_ec 11
isr_ec 12
isr_ec 13
isr_ec 14
isr 15
isr 16
isr 17
isr 18

isr_routine:
  pushad
  push ds
  push es
  push fs
  push gs
  push esp      ; as parameter of isr_dispatcher
  call _isr_dispatcher
  pop esp
  pop gs
  pop fs
  pop es
  pop ds
  popad
  add esp, 0x8  ; pop error code & exception index
  iret
