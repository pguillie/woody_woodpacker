;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_strdup.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/21 18:48:53 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:32:25 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_strdup
extern malloc, ft_strlen, ft_memcpy

section .text
ft_strdup:
	push	rbp
	mov	rbp, rsp
	push	rdi
	call	ft_strlen
	mov	rdi, rax
	push	rdi
	call	malloc
	test	rax, rax
	jz	return
	pop	rdx
	pop	rsi
	mov	rdi, rax
	call	ft_memcpy
return:
	leave
	ret
