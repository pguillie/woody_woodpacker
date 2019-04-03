;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_memcmp.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/03/01 15:07:32 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 14:53:32 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_memcmp

section .text
ft_memcmp:
	push	rbp
	mov	rbp, rsp
	mov	rcx, rdx
	xor	rax, rax
	repe
	cmpsb
	jz	return
	movzx	rdx, byte [rdi - 1]
	movzx	rcx, byte [rsi - 1]
	mov	rax, rdx
	sub	rax, rcx
return:
	leave
	ret
