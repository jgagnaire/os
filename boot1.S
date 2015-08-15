;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;		    JG			   ;
;					   ;
; bootloader stage 2 - boot1		   ;
;					   ;
; the purpose of boot1 is to init the GDT, ;
; switch to protected mode, and jump to    ;
; the kernel				   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
[ORG 0x0]

	jmp boot1

%include "my_putstr.inc"

boot1:
	mov [bootdev], dl

	mov ax, 0x100		; Initializes segment registers
	mov ds, ax
	mov es, ax

	mov ax, 0x8000		; Sets a stack
	mov ss, ax
	mov sp, 0xF000

	mov si, test_msg
	call my_putstr

	push es			; Prepares the jump to kernel location
	mov ax, 0x120
	mov es, ax
	xor bx, bx

	mov ah, 2
	mov al, 1
	xor ch, ch
	mov cl, 3
	xor dh, dh
	mov dl, [bootdev]
	int 0x13
	pop es

	jmp 0x120:0		; Jumps to kernel


bootdev	db 0
test_msg db "Executing Boot1...",13,10,0

	times 512-($-$$) db 0
