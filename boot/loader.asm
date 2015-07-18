load:
	push bp
	mov bp, sp
	pusha
	
	mov ax, [bp + 4] ; sectors to read
	xor bx, bx
	mov dx, [bp + 6] ; load to where?
	mov cx, [bp + 8] ; start sector

load_loop:
	cmp ax, 0
	je load_ok
	push cx
	push dx
	push bx
	call read_sector
	add sp, 6
	dec ax
	add bx, 512
	inc cx
	cmp bx, 0x1000
	jne load_loop
	add dx, 0x100
	xor bx, bx
	jmp load_loop
	
load_ok:
	push disk_ok_msg
	call print
	add sp, 2
	popa
	pop bp
	ret

; read a sector from the floppy drive.
; parameters: destination offset, destination segment, logical sector number
read_sector:
	push bp
	mov bp, sp
	pusha

	mov ax, [bp + 8]
	xor dx, dx
	mov bx, sectors_per_track
	div bx
	inc dx                  ; sector is 1-based
	mov [sector_index], dx
	and ax, 1               ; total_heads = 2
	mov [head_index], ax
	
	mov ax, [bp + 8]
	xor dx, dx
	mov bx, sectors_per_track * total_heads
	div bx
	mov [cylinder_index], ax

	mov [number_retries], word 0
	
read_loop:
	mov ax, [bp + 6]
	mov es, ax
	mov dl, [boot_drive]
	mov ah, 0x2
	mov al, 0x1
	mov bx, [cylinder_index]
	mov ch, bl
	mov bx, [sector_index]
	mov cl, bl
	mov bx, [head_index]
	mov dh, bl
	mov bx, [bp + 4]
	int 0x13
	jnc read_ok

	inc word [number_retries]
	cmp word [number_retries], 3  ; retry 3 times
	jne read_loop

	push disk_error_msg
	call print
	cli
	hlt

read_ok:
	popa
	pop bp
	ret

sector_index dw 0
head_index dw 0
cylinder_index dw 0
number_retries dw 0

sectors_per_track  equ 18
total_heads        equ 2
total_cylinders    equ 80
