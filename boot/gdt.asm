gdt:

.start:

.null:
  dd 0x0
  dd 0x0

.code: equ $ - gdt
  dw 0xffff     ; limit
  dw 0x0        ; base_low
  db 0x0        ; base_middle
  db 10011010b  ; access
  db 11001111b  ; granularity
  db 0x0        ; base_high

.data: equ $ - gdt
  dw 0xffff     ; limit
  dw 0x0        ; base_low
  db 0x0        ; base_middle
  db 10010010b  ; access
  db 11001111b  ; granularity
  db 0x0        ; base_high

.end:

.descriptor:
  dw .end - .start - 1  ; limit
  dd .start             ; base
