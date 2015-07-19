[global begin]
[extern mem_detect]
[extern _init_paging]
[extern gdt_descriptor]
[extern code_segment]
[extern data_segment]
[extern print]
[bits 16]

begin:
	; detect memory via int 0x15, eax=0xe820
	call mem_detect
	
	; setup pde & pte (4 entries)
	push dword begin2
	jmp _init_paging
	
begin2:
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

	; enable paging & protected mode
	mov eax, 0x1000  ; 0x1000 is the address of page directory entry
	mov cr3, eax
	mov eax, cr0
	or  eax, 0x80000001
	mov cr0, eax

	; switch to protected mode
	jmp dword code_segment : protected_mode

[bits 32]

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
	jmp 0xC0000000

[section .data]

protected_mode_msg db "Switching to the protected mode.", 0
