res:
	push	rax
	push	rdi
	push	rsi
	push	rdx

	call	hw
	db	"Hello, World!", 0xa

hw:
	mov	rax, 0x1
	mov	rdi, 0x1
	pop	rsi
	mov	rdx, 0xe
	syscall

	pop	rdx
	pop	rsi
	pop	rdi
	pop	rax

	nop
	nop
	jmp	0x42424246
