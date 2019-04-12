foo:
	push	rax
	push	rdi
	push	rsi
	push	rdx

	lea	rdi, [rel foo]
	xor	rax, rax
bar:
	xor	byte [rdi + rax], 0x42
	inc	rax
	cmp	rax, 0x42424242
	jl	bar

	call	baz
	db	"....WOODY....", 0xa

baz:
	mov	rax, 0x1
	mov	rdi, 0x1
	pop	rsi
	mov	rdx, 0xe
	syscall

	pop	rdx
	pop	rsi
	pop	rdi
	pop	rax
	jmp	0x42424246
