[org 0x7c00]
[bits 16]

jmp 0x0 : start_16
start_16:

; initialize segment registers & stack
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
mov bp, 0x8000
mov sp, bp

; store boot drive
mov [boot_drive], dl

; display the booting message
mov si, booting_msg
call print

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
jne disk_fatal

; jump to kernel
mov si, disk_ok_msg
call print
jmp 0:0x9000

disk_fatal:
	mov si, disk_fl_msg
	call print
	cli
	hlt

; utilities
%include "print.asm"

; floppy reader
%include "floppy.asm"

boot_drive     db 0
booting_msg    db "Booting.", 0
disk_ok_msg    db "Kernel is loaded, jumping.", 0
disk_fl_msg    db "Disk error.", 0

times 510 - ($ - $$) db 0
dw 0xaa55
