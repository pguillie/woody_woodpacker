res:
	call	hw
	db	"Hello, World!", 0xa

hw:
	mov	rax, 0x1
	mov	rdi, 0x1
	pop	rsi
	mov	rdx, 0xe
	syscall
	nop
	nop
	jmp	0x42424246
