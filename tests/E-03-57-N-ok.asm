; BSS
segment	.bss
; ALIGN
align	4
; LABEL x
x:
; SALLOC 4
	resb	4
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
; INT 9
	push	dword 9
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; INT 1
	push	dword 1
; DUP32
	push	dword [esp]
; ADDR x
	push	dword $x
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; LABEL _L1
_L1:
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; INT 3
	push	dword 3
; NEG
	neg	dword [esp]
; LT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setl	cl
	mov	[esp], ecx
; JZ _L2
	pop	eax
	cmp	eax, byte 0
	je	near _L2
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL _L3
_L3:
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; DUP32
	push	dword [esp]
; ADDR x
	push	dword $x
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; JMP _L1
	jmp	dword _L1
; LABEL _L2
_L2:
; LABEL _L4
_L4:
; LABEL _L5
_L5:
        ;; after body 
; LOCV -4
	push	dword [ebp-4]
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printi
extern	printi
