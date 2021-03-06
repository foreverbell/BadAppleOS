[org 0x7c00]
[bits 16]

entry:
  ; jmp +0x4
  dw 0xeb04

  ; populated by link.py
  kernel_sectors dw 0

  ; clear cs
  jmp 0x0:start_16

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

  ; enable A20 line
  in al, 0x92
  or al, 0x2
  out 0x92, al

  ; detect floppy or disk
  call detect

  ; load kernel to 0x10000
  mov ax, [kernel_sectors]
  push 1
  push 0x1000
  push ax
  call load
  add sp, 6

  ; load gdt
  cli
  lgdt [gdt.descriptor]

  ; enable protection
  mov eax, cr0
  or  eax, 0x1
  mov cr0, eax

  ; switch to protected mode
  jmp dword gdt.code:start_32

[bits 32]

start_32:
  ; initialize segment registers
  mov ax, gdt.data
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; execute kernel, skip 0xc bytes for multi-boot header
  jmp 0x1000c

[bits 16]

disk_fatal:
  mov si, disk_fl_msg
  call print
  cli
  hlt

%include "print.asm"
%include "disk.asm"
%include "gdt.asm"

boot_drive  db 0
booting_msg db "Booting.", 0
disk_fl_msg db "Disk error.", 0

times 510 - ($ - $$) db 0
dw 0xaa55
