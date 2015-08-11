[BITS 16]
[ORG  0x7C00]			; prgm is located at 0x7C00

	cli
	jmp boot

msg	db 'leling ...',0

display_msg:
	lodsb			; moves a byte from ds:si to al
				; DS = Data Segment, SI = Source Index,
				; see wikipedia "segmented adressing"
	mov ah, 0x13		; number of the 'write-a-string' function
	mov al, 1		; write mode (advance cursor, ASCII string)
	mov bh, 0		; page number to write to,
				; 0 <=> screen (see framebuffer on lse's website)
	mov bl, 0x02		; text attribute (0x02 = green on black)
	mov cx, 10		; string length
	mov dh, 12		; starting row
	mov dl, 30		; starting column
	mov bp, msg

	int 0x10		; call the BIOS video interrupt

boot:
	call display_msg
	jmp $
	times 510-($-$$) db 0 	; filling the rest with 0s.
				; '$' <=> 'start of this instr', '$$' <=> 'start of this prgm'
				; 510 because the bootsector has to be 512 bytes long
	dw 0xAA55		; bootsector signature (510 + 2 = 512 bytes)







	




reboot:
	db 0xEA			; LOL, 5 bytes in memory and the computer reboots.
	dw 0x0000
	dw 0xFFFF

