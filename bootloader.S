;
; JG - bootloader.S
;

[BITS 16]
[ORG 0x0]

	cli			; Disables CPU hardware interrupts so that the stack
				; stays untouched
	jmp boot

%include "my_putstr.inc"

boot:
	mov [bootdev], dl	; Gets the boot device nb setted by the BIOS in dl

	mov ax, 0x07C0		; Initializes the segment registers to 0x7C00 in RAM
	mov ds, ax		; since the BIOS will call the boot sector from here
	mov es, ax

	mov ax, 0x8000		; Sets a stack from 0x8F000 to 0x80000 in RAM
	mov ss, ax		; We use ax as a temporary register since we can't modify
	mov sp, 0xF000		; directly the ss register

	mov si, msg		; Argument string to display
	call my_putstr

	xor ax, ax		; Clears ah and al - faster than 'mov ax, 0'
	int 0x13		; Resets disk in the dl register - see 'int 13h ah=0'

	sti			; Enables CPU hardware interrupts back

	push es			; Saves es on the stack
	mov ax, 0x100		; We want to copy the kernel at physical location 0x1000
	mov es, ax		; Sets a segment at 0x1000 in RAM
	xor bx, bx

	mov ah, 2		; Int 13h read function number
	mov al, 50		; 50 sectors are to be read
	xor ch, ch		; On cylinder 0
	mov cl, 2		; We want to read from sector 2 (sector nb starts at 1, not 0)
	xor dh, dh		; At head 0
	mov dl, [bootdev]	; Get the previously saved boot device nb from the BIOS
	int 0x13		; Calls the interrupt
	pop es			; Restores es from the stack

	mov ax, gdtend
	mov bx, gdt
	sub ax, bx		; Stores the size of the GDT (end - begin) in ax,
	mov word [gdtptr], ax	; and finally in gdtptr, to be loaded in GDTR

	xor eax, eax		; Calculates (segment address * 16 + offset) to get the linear
	xor ebx, ebx		; address of the GDT (which starts at address 'gdt')
	mov ax, ds		; Starts from the data segment address 0x7C0 stored in ds
	mov ecx, eax		; ecx is now equals to 0x7C0
	shl ecx, 4		; 4-bit left shift = ecx * 16 - see note on memory segmentation
	mov bx, gdt
	add ecx, ebx		; Adds the offset to the segment address to get the linear address
	mov dword [gdtptr + 2], ecx ; Stores the GDT linear address in the second part of the
				; memory area pointed by gdtptr, in order to load it in GDTR

	cli			; Disables interrupts since the way of addressing memory
				; will change after the switch in protected mode

	lgdt [gdtptr]		; Loads the values pointed by gdtptr, eg the GDT size (limit) and
				; base addresses, in the GDT register - GDTR, of size 6 bytes

	mov eax, cr0		; Switching to protected mode - since we can't directly modify
	or ax, 1		; cr0, we use a temporary register, and set its first bit to 1.
	mov cr0, eax		; cr0's first bit is called the PE (Protection Enable) bit
				; ax is used to access the 16 low bits of eax

	jmp next_line		; Empties the processor internal caches

next_line:
	mov ax, 0x10		; Resets all the segment registers to the segment descripted
	mov ds, ax		; in the GDT at offset 0x10 = 16 bytes - at which resides the
	mov fs, ax		; data segment descriptor in the GDT
	mov gs, ax
	mov es, ax
	mov ss, ax		; Same with the stack segment
	mov esp, 0x9F000	; Sets the stack pointer

	jmp dword 0x8:0x1000	; Finally jumps to the kernel - 0x8 is the offset in the GDT
				; corresponding to the code segment


msg	db "Executing bootloader...",13,10,0
bootdev	db 0

gdt:				; GDT itself - The first descriptor, called the NULL descriptor, is
	db 0,0,0,0,0,0,0,0	; is never used and setted to zeros.
gdt_cs:				; The second describes the code segment
	db 0xFF,0xFF,0x0,0x0,0x0,10011011b,11011111b,0x0
gdt_ds:				; The third describes the data segment
	db 0xFF,0xFF,0x0,0x0,0x0,10010011b,11011111b,0x0
gdtend:				; This is for the calculation of the GDT size


gdtptr:
	dw 0			; 2 bytes representing the limit of the GDT
	dd 0			; 4 bytes representing the address base of the GDT
				; 6 bytes is the size of the GDTR

	times 510-($-$$) db 0	; Fills the rest with 0s.
				; '$' = 'start of this instruction', '$$' = 'start of this prgm'
				; 510 because the boot sector has to be 512 bytes long

	dw 0xAA55		; Bootloader signature (510 + 2 bytes), so that the BIOS
				; recognizes this program as a boot sector
