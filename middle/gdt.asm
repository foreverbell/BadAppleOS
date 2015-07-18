[bits 16]

[global gdt_descriptor]
[global code_segment]
[global data_segment]

gdt_start:

gdt_dummy:
	dd 0x0
	dd 0x0

gdt_code:
	dw 0xffff     ; limit
	dw 0x0        ; base_low
	db 0x0        ; base_middle
	db 10011010b  ; access
	db 11001111b  ; granularity
	db 0x0        ; base_high

gdt_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1  ; limit
	dd gdt_start                ; base

code_segment equ gdt_code - gdt_start
data_segment equ gdt_data - gdt_start
