;******************************************************************************;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    ft_strcat.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: pguillie <pguillie@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/21 18:39:04 by pguillie          #+#    #+#              ;
;    Updated: 2019/03/04 15:29:02 by pguillie         ###   ########.fr        ;
;                                                                              ;
;******************************************************************************;

global ft_strcat

section .text
ft_strcat:
	push	rbp
	mov	rbp, rsp
	mov	rax, rdi
	xor	rcx, rcx
offset:
	mov	dl, byte [rdi]
	test	dl, dl
	jz	copy
	inc	rdi
	jmp	offset
copy:
	mov	dl, byte [rsi + rcx]
	mov	byte [rdi], dl
	test	dl, dl
	jz	return
	inc	rdi
	inc	rcx
	jmp	copy
return:
	leave
	ret
