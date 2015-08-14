;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;		   JG			;
;					;
; MBR - boot0				;
;					;
; the purpose of boot0 is to read boot1 ;
; from the floppy drive and jmp to it	;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]			; 16 bits ASM in real-mode
[ORG  0x7C00]			; BIOS jumps at 0x7C00, so the MBR has
				; to be located at this adress

	cli			; Disables CPU hardware interrupts
				; to avoid data to be pushed on the stack
	jmp boot0

msg	db 'Loading ...', 0

my_putstr:		   	; Displays a message in ds:si to the screen
	lodsb
	cmp al, 0
	je done
	mov ah, 0x0E
	mov bx, 0x07
	int 0x10		; Calls the BIOS video interrupt
	jmp my_putstr

done:
	ret

reset_floppy:			; Reset floppy drive controller at the begin of sector 0
	xor ah, ah		; Reset function of BIOS int 0x13 number
	xor dl, dl		; 1st floppy drive
	int 0x13		; BIOS interrupt for drives read/write operations
	jc reset_floppy		; jc = jump-if-carry (is set) = if BIOS int 0x13 failed

read_floppy:			; See wikipedia 'int 13h AH=02h' for possible parameters
	mov ah, 0x02		; Read function of BIOS int 0x13 nb
	mov al, 1		; Reads 1 sector (512 bytes)
	mov ch, 1		; Cylinder 1
	mov cl, 2		; Reads the second sector (sector nb starts at 1 and not 0)
	xor dh, dh		; Head 0
	xor dl, dl		; Drive 0 = floppy drive
	int 0x13
	jc read_floppy

boot0:
	mov si, msg
	call my_putstr

	xor ax, ax		; Adjusts segment registers in 0x7C00 -
	mov ds, ax		; theoretically overlaps the stack, but not in practice	
	mov es, ax		; because boot0 won't use enough stack space to overwrite
	mov gs, ax		; the code

	xor ax, ax		; Creates stack of size 65535 - biggest addressable space
				; in real-mode (16 bits ASM)
	mov ss, ax		; ss = base pointer
	mov sp, 0xFFFF		; Register sp points to the top of the stack

	sti			; Enables back CPU hardware interrupts

	call reset_floppy	; See the physical architecture of a floppy to enlight
				; what cylinders, heads, tracks and sectors are, and
				; why actions performed on them can fail

	mov ax, 0x1000		; To read the boot1 sector at arbitrary adress 0x1000
	mov es, ax		; A 64kb segment starts at the adress contained in es
	xor bx, bx	 	; Clears bx register - faster than mov bx, 0

	call read_floppy
	
	jmp 0x1000		; Finally jumps to boot1

	hlt			; Halts the system

	times 510-($-$$) db 0 	; Fills the rest with 0s.
				; '$' = 'start of this instruction', '$$' = 'start of this prgm'
				; 510 because the bootsector has to be 512 bytes long

	dw 0xAA55		; Bootloader signature (510 + 2 = 512 bytes), so that the BIOS
				; recognizes boot0 as an MBR
