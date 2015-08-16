;
; JG - my_putstr.inc
;

%ifndef MY_PUTSTR_INC
%define MY_PUTSTR_INC

my_putstr:
	push ax			; Saves ax and bx
	push bx
begin:
	lodsb			; Loads ds:si in al
	cmp al, 0		; Check if al contains the terminating null byte
	je end			; Returns if so
	mov ah, 0x0E		; Function nb to display a character to the screen
	mov bx, 0x07		; Text attributes - ASCII, white on black background
	int 0x10		; Calls the BIOS video interrupt
	jmp begin		; Again for the next character

end:
	pop bx			; Restores bx and ax
	pop ax
	ret

%endif