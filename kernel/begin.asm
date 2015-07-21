[global begin]
[extern _kinitialize]
[section .text]
[bits 32]

begin:
	call _kinitialize
	jmp loop

loop:
	hlt
	jmp loop