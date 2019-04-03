;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_isascii.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/21 18:52:59 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:37:21 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_isascii

section .text
ft_isascii:
	push	rbp
	mov	rbp, rsp
	mov	rax, -0x80
	and	rdi, rax
	xor	rax, rax
	test	rdi, rdi
	setz	al
	leave
	ret
