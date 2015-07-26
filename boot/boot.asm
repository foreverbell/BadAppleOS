[org 0x7c00]

[bits 16]

; clear data segment
xor ax, ax
mov ds, ax

; init the kernel stack
mov bp, 0x8000
mov sp, bp

; store boot drive
mov [boot_drive], dl

; display the booting message
push booting_msg
call print
add sp, 2

call detect

; load middle to 0x9000, 16 sectors
push 1
push 0x900
push 16
call load
add sp, 6

; load kernel to 0x10000, 1024 sectors
push 17
push 0x1000
push 1024
call load
add sp, 6

; check if magic number is correct
mov ax, 0xc031
cmp ax, [0x9000]
je go
push disk_fl_msg
call print
cli
hlt

go:
	; give up control to kernel
	push disk_ok_msg
	call print
	add sp, 2
	jmp 0x9000

; utilities
%include "print.asm"

; floppy reader
%include "floppy.asm"

boot_drive     db 0
booting_msg    db "Booting...", 0
detect_fl_msg  db "Detect drive parameters failed, use floppy as default.", 0
disk_ok_msg    db "Kernel is loaded, jumping to kernel.", 0
disk_fl_msg    db "Disk read error! System halted.", 0

times 510 - ($ - $$) db 0
dw 0xaa55
