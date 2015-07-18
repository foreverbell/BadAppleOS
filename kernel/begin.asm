[global begin]
[extern _kernel_init]
[section .text]
[bits 32]

begin:
	call _kernel_init

loop:
	hlt
	jmp loop