[global begin]
[extern _mem_detect]
[extern gdt_descriptor]
[extern code_segment]
[extern data_segment]

[section .text]

[bits 16]

begin:

; detect memory via int 0x15, eax=0xe820
xor ax, ax
mov es, ax
push dword 128
push dword 0x8004
push word 0            ; evil hack
push mem_detect_ret
jmp _mem_detect

mem_detect_ret:
add sp, 10
cmp eax, 0x0
jge mem_detect_ok

mem_detect_err:
	push mem_detect_err_msg
	call print
	hlt

mem_detect_ok:
	mov [0x8000], eax

; display the protected mode message
push protected_mode_msg
call print
add sp, 2

; load gdt
cli
lgdt [gdt_descriptor]

; enable A20 line
in al, 0x92
or al, 0x2
out 0x92, al

; switch to the protected mode
mov eax, cr0
or eax, 0x1
mov cr0, eax
jmp dword code_segment : protected_mode

; utility
%include "../boot/print.asm"

[bits 32]

; protected mode
protected_mode:
	; initialize segment registers
	mov ax, data_segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	; initialize kernel stack
	mov ebp, 0x10000
	mov esp, ebp
	; execute kernel
	jmp 0x10000

[section .data]

protected_mode_msg db "Switching to the protected mode.", 0
mem_detect_err_msg db "Memory detection error! System halted.", 0
