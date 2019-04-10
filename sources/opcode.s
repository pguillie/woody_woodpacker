foo:
	push	rdi
	push	rcx
	push	rdx

	lea	rdi, [rel bar]
	xor	rcx, rcx
bar:
	xor	byte [rdi + rcx], 0x42
	inc	rcx
	cmp	rcx, 0x1000
	jl	bar

	pop	rdx
	pop	rcx
	pop	rdi
	jmp	0x42424246
