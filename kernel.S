;
; JG - kernel.S
;

[BITS 32]
[ORG 0x1000]

	mov byte [0xB8A00], 'L'
	mov byte [0xB8A01], 000101b
	mov byte [0xB8A02], 'E'
	mov byte [0xB8A03], 000101b
	mov byte [0xB8A04], 'L'
	mov byte [0xB8A05], 000101b

	jmp $
