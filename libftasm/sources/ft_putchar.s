;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_putchar.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/03/02 18:26:39 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:05:02 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_putchar

section .text
ft_putchar:
	push	rbp
	mov	rbp, rsp
	push	rdi
	mov	rax, 0x2000004
	mov	rdi, 0x1
	lea	rsi, [rsp]
	mov	rdx, 0x1
	syscall
	leave
	ret
