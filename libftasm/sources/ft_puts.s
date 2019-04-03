;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_puts.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/21 18:42:35 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:26:30 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_puts
extern ft_strlen

section .text
ft_puts:
	push	rbp
	mov	rbp, rsp
	test	rdi, rdi
	jz	null_arg
	push	rdi
	call	ft_strlen
	mov	rdx, rax
	mov	rax, 0x2000004
	mov	rdi, 0x1
	pop	rsi
	syscall
	jc	error
	mov	rax, 0x2000004
	mov	rdi, 0x1
	lea	rsi, [rel nl]
	mov	rdx, 0x1
	syscall
	jc	error
	jmp	return
null_arg:
	mov	rax, 0x2000004
	mov	rdi, 0x1
	lea	rsi, [rel null]
	mov	rdx, null.len
	syscall
	jc	error
	jmp	return
error:
	mov	rax, -0x1
	leave
	ret
return:
	mov	rax, 0x10
	leave
	ret

section	.rodata
null:
	db "(null)", 0xa
.len: equ $ - null
nl:
	db 0xa
