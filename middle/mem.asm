[global mem_detect]
[extern _mem_detect_cc]
[extern print]

[bits 16]

mem_detect:
	xor ax, ax
	mov es, ax
	push dword 128
	push dword 0x8004
	push word 0            ; evil hack
	push mem_detect_ret
	jmp _mem_detect_cc

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
	ret
	
mem_detect_err_msg db "Memory detection error! System halted.", 0
