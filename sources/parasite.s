global parasite, parasite_end, hash, encrypt

section .text
parasite:
	push	rax
	push	rdi
	push	rsi
	push	rdx
	push	rcx
	push	r8
	push	r9
	jmp	entry
;; DJB2 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
hash:
	push	rbp
	mov	rbp, rsp
	mov	rax, 5381
	test	rsi, rsi
	jz	hashret
	xor	rcx, rcx
hashloop:
	mov	rdx, rax
	shl	rax, 5
	add	rax, rdx
	movzx	rdx, byte [rdi]
	add	rax, rdx
	inc	rcx
	inc	rdi
	cmp	rcx, rsi
	jl	hashloop
hashret:
	leave
	ret
;; ARC4 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
encrypt:
	push	rbp
	mov	rbp, rsp
	test	rsi, rsi
	jz	encrret
	sub	rsp, 0x118
	mov	qword [rbp - 0x108], rdi
	mov	qword [rbp - 0x110], rsi
	mov	qword [rbp - 0x118], rdx

	lea	rsi, [rbp - 0x100]
	xor	rcx, rcx
ksa1:
	mov	byte [rsi + rcx], cl
	inc	rcx
	cmp	rcx, 0x100
	jb	ksa1

	lea	rdi, [rbp - 0x118]
	xor	rcx, rcx
	xor	rax, rax
	xor	rdx, rdx
ksa2:
	mov	dl, byte [rsi + rcx]
	add	rax, rdx
	mov	rdx, rcx
	and	rdx, 0x7
	mov	dl, byte [rdi + rdx]
	add	rax, rdx
	and	rax, 0xff
	mov	dl, byte [rsi + rcx]
	xchg	byte [rsi + rax], dl
	mov	byte [rsi + rcx], dl
	inc	rcx
	cmp	rcx, 0x100
	jb	ksa2

	mov	rdi, [rbp - 0x108]
	mov	rcx, [rbp - 0x110]
	xor	r8, r8
	xor	r9, r9
	xor	rax, rax
	xor	rdx, rdx
prga:
	inc	r8
	and	r8, 0xff
	mov	dl, byte [rsi + r8]
	add	r9, rdx
	and	r9, 0xff
	mov	dl, byte [rsi + r8]
	xchg	byte [rsi + r9], dl
	mov	byte [rsi + r8], dl
	mov	al, byte [rsi + r8]
	mov	dl, byte [rsi + r9]
	add	rax, rdx
	and	rax, 0xff
	mov	al, byte [rsi + rax]
	xor	byte [rdi], al
	inc	rdi
	dec	rcx
	test	rcx, rcx
	jnz	prga

encrret:
	leave
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
entry:
;; hash
	lea	rdi, [rel parasite]
	mov	rsi, 0x42424242	;parasite length
	call	hash
;; decrypt
	lea	rdi, [rel parasite] ;.text section
	mov	rsi, 0x42424242	    ;.text section length
	mov	rdx, rax
	call	encrypt
;; signature
	call	woody
	db	"....WOODY....", 0xa
woody:
	mov	rax, 0x1
	mov	rdi, 0x1
	pop	rsi
	mov	rdx, 0xe
	syscall

	pop	r9
	pop	r8
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	pop	rax
	jmp	0x42424246	; old entry point
parasite_end:
