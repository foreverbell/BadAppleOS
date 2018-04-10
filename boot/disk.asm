; Refer to http://wiki.osdev.org/ATA_in_x86_RealMode_(BIOS) for full
; documentation.

; @function: detect
; @brief: detect drive parameters via int 13h, ah=8h.
; @parameters: none.
detect:
  pusha
  xor ax, ax
  mov es, ax
  mov di, ax
  mov ah, 0x8
  mov dl, [boot_drive]
  int 0x13
  jnc .ok
  popa
  ret  ; use default floopy

.ok:
  mov dl, dh
  xor dh, dh
  inc dx
  mov [total_heads], dx
  mov ax, cx
  and ax, 0x3f
  mov [sectors_per_track], ax
  popa
  ret

; @function: load
; @brief: load the kernel image to memory.
; @parameters: start sector, destination, the number of sectors to read.
load:
  push bp
  mov bp, sp
  pusha

  mov ax, [bp + 4]  ; sectors to read
  xor bx, bx
  mov dx, [bp + 6]  ; load to where?
  mov cx, [bp + 8]  ; start sector

.loop:
  cmp ax, 0
  je .ok
  push cx
  push dx
  push bx
  call read
  add sp, 6
  dec ax
  add bx, 0x200
  inc cx
  cmp bx, 0x1000
  jne .loop
  add dx, 0x100
  xor bx, bx
  jmp .loop

.ok:
  popa
  pop bp
  ret

; @function: read
; @brief: read a sector from drive.
; @parameters: buffer offset, buffer segment, LBA.
read:
  push bp
  mov bp, sp
  pusha

  mov ax, [bp + 8]
  xor dx, dx
  mov bx, [sectors_per_track]
  div bx
  inc dx  ; sector is 1-based
  mov [sector_index], dx
  xor dx, dx
  mov bx, [total_heads]
  div bx
  mov [head_index], dx
  mov [cylinder_index], ax

  mov [number_retries], byte 0

  ; read a sector
  mov bx, [cylinder_index]
  mov ch, bl
  mov bx, [sector_index]
  mov cl, bl
  mov bx, [cylinder_index]
  shr bx, 2
  and bx, 0xc0
  or  cl, bl  ; not necessary for floppy
  mov bx, [head_index]
  mov dh, bl
  mov bx, [bp + 4]
  mov es, [bp + 6]
  mov dl, [boot_drive]

.loop:
  mov ah, 0x2
  mov al, 0x1
  int 0x13
  jnc .ok

  ; reset disk drive
  xor ax, ax
  int 0x13

  inc byte [number_retries]
  cmp byte [number_retries], 5  ; retry 5 times

  jne .loop
  jmp disk_fatal  ; see boot.asm

.ok:
  popa
  pop bp
  ret

sector_index       dw 0
head_index         dw 0
cylinder_index     dw 0
number_retries     db 0

; floppy default value (place holder, can be overwritten)
sectors_per_track  dw 18
total_heads        dw 2
