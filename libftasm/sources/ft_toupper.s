;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_toupper.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/21 18:51:05 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:34:30 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_toupper
extern ft_islower

section .text
ft_toupper:
	push	rbp
	mov	rbp, rsp
	push	rdi
	call	ft_islower
	pop	rdi
	test	rax, rax
	jz	return
	mov	rax, 0x20
	not	rax
	and	rdi, rax
return:
	mov	rax, rdi
	leave
	ret
