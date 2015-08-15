;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 		   JG		  ;
;				  ;
;  MBR - boot0			  ;
;				  ;
; the purpose of boot0 is to jump ;
; to boot1			  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
[ORG 0x0]

	jmp boot0

%include "my_putstr.inc"

boot0:
	mov [bootdev], dl	; Gets the boot device nb setted by the BIOS in dl

	mov ax, 0x07C0		; Initializes the segment registers in 0x7C00
	mov ds, ax		; since the BIOS will call the MBR from here
	mov es, ax

	mov ax, 0x8000		; Sets a stack from 0x8F000 to 0x80000 in RAM
	mov ss, ax
	mov sp, 0xF000

	mov si, msg		; Argument string to display
	call my_putstr

	xor ax, ax		; Clears ah and al
	int 0x13		; Resets disk in the dl register - see 'int13h ah=0'
	
	push es			; Saves es on the stack
	mov ax, 0x100		; We want to copy boot1 at physical location 0x1000
	mov es, ax		; Sets a segment at 0x1000 in RAM
	xor bx, bx		; Clears bx register - faster than 'mov bx, 0'

	mov ah, 2		; Int 13h read function number
	mov al, 1		; 1 sector is to be read
	xor ch, ch		; On cylinder 0
	mov cl, 2		; We want to read sector 2
	xor dh, dh		; At head 0
	mov dl, [bootdev]	; Get the previously saved boot device nb from the BIOS
	int 0x13		; Calls the interrupt
	pop es			; Restores es from the stack

	jmp dword 0x100:0	; Finally jumps to boot1

	hlt


msg	db "Loading ...", 0
bootdev	db 0

	times 510-($-$$) db 0	; Fills the rest with 0s.
				; '$' = 'start of this instruction', '$$' = 'start of this prgm'
				; 510 because the bootsector has to be 512 bytes long

	dw 0xAA55		; Bootloader signature (510 + 2 bytes), so that the BIOS
				; recognizes boot0 as an MBR
