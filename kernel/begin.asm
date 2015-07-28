[global begin]
[extern _kinitialize]
[section .text]
[bits 32]

begin:
	jmp _kinitialize
	; never be here