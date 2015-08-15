
[BITS 16]
[ORG 0x0]

	jmp start

%include "my_putstr.inc"

start:
	mov ax, 0x120
	mov ds, ax
	mov es, ax

	mov ax, 0x8000
	mov ss, ax
	mov sp, 0xF000

	mov si, test_k
	call my_putstr

	jmp $

test_k db "msg from the kernel",13,10,0
