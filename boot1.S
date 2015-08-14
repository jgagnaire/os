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
[ORG 0x1000]

	jmp boot1

boot1:
	mov ax, $$		; Initializes segment registers - see boot0.S for details
	mov ds, ax
	mov es, ax
	mov gs, ax

	mov ax, 0x0		; Sets a stack in 0x1000 of size 65535 - see boot0.S
	mov ss, ax
	mov sp, 0xFFFF
