; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 4
	push	ebp
	mov	ebp, esp
	sub	esp, 4
; INT 0
	push	dword 0
; LOCA -4
	pop	eax
	mov	[ebp-4], eax
        ;; before body 
; INT 0
	push	dword 0
; JZ _L1
	pop	eax
	cmp	eax, byte 0
	je	near _L1
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L2
_L2:
; SSTRING (value omitted -- see below)
	db	"KO", 0
; TEXT
segment	.text
; ADDR _L2
	push	dword $_L2
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; LABEL _L1
_L1:
; INT 1
	push	dword 1
; JZ _L3
	pop	eax
	cmp	eax, byte 0
	je	near _L3
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L4
_L4:
; SSTRING (value omitted -- see below)
	db	"elif", 0
; TEXT
segment	.text
; ADDR _L4
	push	dword $_L4
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L5
	jmp	dword _L5
; LABEL _L3
_L3:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L6
_L6:
; SSTRING (value omitted -- see below)
	db	"OK", 0
; TEXT
segment	.text
; ADDR _L6
	push	dword $_L6
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; LABEL _L5
_L5:
; LABEL _L7
_L7:
; LABEL _L8
_L8:
        ;; after body 
; LOCV -4
	push	dword [ebp-4]
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN prints
extern	prints
