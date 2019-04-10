foo:
	push	rdi
	push	rcx
	push	rdx

	lea	rdi, [rel bar]
	xor	rcx, rcx
bar:
	dec	byte [rdi + rcx]
	inc	rcx
	cmp	rcx, 0x10
	jl	bar

	pop	rdx
	pop	rcx
	pop	rdi
	jmp	0x42424246
