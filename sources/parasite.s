global parasite, parasite_end, hash, encrypt

section .text
parasite:
	push	rax
	push	rdi
	push	rsi
	push	rdx
	push	rcx
	jmp	entry
;; djb2 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
;; rc4 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; encrypt:
; 	push	rbp
; 	mov	rbp, rsp
; 	test	rsi, rsi
; 	jz	encrret
; 	xor	rcx, rcx
; encrloop:
; 	xor	byte [rdi], dl
; 	inc	rcx
; 	inc	rdi
; 	cmp	rcx, rsi
; 	jl	encrloop
; encrret:
; 	leave
; 	ret

;;void encrypt(char *data, size_t len, char *key);
encrypt:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0x118
	push	rbx
	mov	qword [rbp - 0x108], rdi

	lea	rdi, [rbp - 0x1000]
	xor	rcx, rcx
ksa1:
	mov	byte [rdi + rcx], cl
	inc	rcx
	cmp	rcx, 0x100
	jb	ksa1

	xor	rax, rax
	xor	rcx, rcx
	xor	rdx, rdx
ksa2:
	add	al, byte [rdi + rcx]
	mov	dl, cl
	and	dl, 0x7
	add	al, byte [rdx + rdx]
	and	rax, 0xff
	mov	dl, byte [rdi + rcx]
	xor	dl, byte [rdi + rax]
	xor	byte [rdi + rax], dl
	xor	dl, byte [rdi + rax]
	mov	byte [rdi + rcx], dl
	inc	rcx
	cmp	rcx, 0x100
	jb	ksa2

	mov	rbx, qword [rbp - 0x108]
	xor	rcx, rcx
	xor	rax, rax
	xor	rdx, rdx
	xor	r8, r8
	xor	r9, r9
prga1:
	cmp	rcx, rsi
	jae	encrret
	inc	r8
	and	r8, 0xff
	mov	al, byte [rdi + r8]
	add	r9, rax
	and	r9, 0xff
	mov	al, byte [rdi + r8]
	xor	al, byte [rdi + r9]
	xor	byte [rdi + r9], al
	xor	al, byte [rdi + r9]
	mov	byte [rdi + r8], al
	mov	dl, byte [rdi + r8]
	add	dl, byte [rdi + r9]
	and	rdx, 0xff
	mov	al, byte [rdi + rcx]
	xor	al, byte [rdi + rdx]
	xor	byte [rdi + rdx], al
	xor	al, byte [rdi + rdx]
	mov	byte [rdi + rcx], al

encrret:
	pop	rbx
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

	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	pop	rax
	jmp	0x42424246	; old entry point
parasite_end:
