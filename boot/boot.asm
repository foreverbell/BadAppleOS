[org 0x7c00]

[bits 16]

; init the kernel stack
mov bp, 0x8000
mov sp, bp

; store boot drive
mov [boot_drive], dl

; init data segment
xor ax, ax
mov ds, ax

; display the booting message
push booting_msg
call print
add sp, 2

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
booting_msg    db "Booting from the 16-bit real mode.", 0
disk_ok_msg    db "Kernel is successfully loaded, transferring control to kernel.", 0
disk_error_msg db "Disk read error while loading kernel! System halted.", 0

times 510 - ($ - $$) db 0
dw 0xaa55
