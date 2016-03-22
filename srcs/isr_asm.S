;
; JG - isr.asm
;

	global asm_default_isr, asm_keybd_isr, asm_clock_isr
	extern default_isr, keybd_isr, clock_isr

asm_default_isr:
	call default_isr
	mov al, 0x20		; EOI is written to the PIC I/O port after execution of the ISR
	out 0x20, al
	iret			; The iret instruction expects the stack to contain (starting by its top) :
				;  -> eip
				;  -> cs
				;  -> the EFLAGS register
				;  -> the stack pointer and the stack segment selector
				; This is because when receiving an interrupt, the CPU pushes them to the stack.
				; iret will pop all of them to resume execution from the new instruction pointer.
				; (see http://www.felixcloutier.com/x86/IRET:IRETD.html for details)
	
asm_keybd_isr:
	call keybd_isr
	mov al, 0x20
	out 0x20, al
	iret


	
asm_clock_isr:
	call clock_isr
	mov al, 0x20
	out 0x20, al
	iret
