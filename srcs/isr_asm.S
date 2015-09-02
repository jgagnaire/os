;
; JG - isr.asm
;

	global asm_default_isr, asm_keybd_isr, asm_clock_isr
	extern default_isr, keybd_isr, clock_isr

asm_default_isr:
	call default_isr
	mov al, 0x20		; EOI is written to the PIC I/O port after execution
	out 0x20, al		; of the ISR

asm_keybd_isr:
	call keybd_isr
	mov al, 0x20
	out 0x20, al
	
asm_clock_isr:
	call clock_isr
	mov al, 0x20
	out 0x20, al

