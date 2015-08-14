;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;		    JG			  ;
;					  ;
; bootloader stage 2 - boot1		  ;
;					  ;
; the purpose of boot1 is to load the	  ;
; kernel from the boot floppy disk, init  ;
; the system and begin kernel execution	  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
[ORG 0x0]

	jmp boot1

%include "my_putstr.inc"

boot1:
	mov ax, 0x100		; Initializes segment registers
	mov ds, ax
	mov es, ax

	mov ax, 0x8000		; Sets a stack
	mov ss, ax
	mov sp, 0xF000

	mov si, test_msg
	call my_putstr

	jmp $

test_msg db "test", 0
