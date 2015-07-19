[global print]
[bits 16]

print:
	push bp
	mov bp, sp
	pusha
	mov bx, [bp + 4]

print_loop:
	mov al, [bx]
	cmp al, 0x0
	je print_fin
	mov ah, 0xe  ; teletype output
	int 0x10
	inc bx
	jmp print_loop

print_fin:
	; print '\n'
	mov ah, 0x3
	xor bh, bh
	int 0x10
	inc dh
	xor dl, dl
	mov ah, 0x2
	int 0x10
	popa
	pop bp
	ret
