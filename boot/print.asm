; @function: print
; @brief: print a string to terminal.
; @parameters: pointer to string is passed through SI.
print:
	pusha
	mov bx, si

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
	ret
