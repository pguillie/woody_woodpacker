global hash, encrypt

;;
extern printf, main
;;

section .text
parasite:
	push	rax
	push	rdi
	push	rsi
	push	rdx
	push	rcx
	jmp	entry
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
encrypt:
	push	rbp
	mov	rbp, rsp
	test	rsi, rsi
	jz	encrret
;;
	push	rdi
	push	rsi
	call	key
	db	"decryption key: %#.16lx", 0xa
key:
	pop	rdi
	mov	rsi, rdx
	call	printf
	pop	rsi
	pop	rdi
;;
	xor	rcx, rcx
encrloop:
	xor	byte [rdi], dl
	inc	rcx
	inc	rdi
	cmp	rcx, rsi
	jl	encrloop
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
