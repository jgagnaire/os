[BITS 16]
[ORG  0x7C00]			; prgm is located at 0x7C00



boot:
	mov ah, 0x0E 		; number of the 'put-a-character-on-the-screen' function
	mov bh, 0x00		; page number
	mov bl, 0x07		; text attribute - 0x07 -> white text, black background
	mov al, 'h'		; put the character in the al register 

	int 0x10		; call the BIOS video interrupt
	jmp $			; endless loop

	times 510-($-$$) db 0 	; filling the rest with 0s.
				; '$' <=> 'start of this instr', '$$' <=> 'start of this prgm'
				; 510 because the last two bytes have to be 0xAA55
	dw 0xAA55		; bootsector signature
