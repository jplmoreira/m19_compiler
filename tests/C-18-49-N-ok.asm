; DATA
segment	.data
; ALIGN
align	4
; LABEL x
x:
; SINT 10
	dd	10
; DATA
segment	.data
; ALIGN
align	4
; LABEL y
y:
; SINT 3
	dd	3
; DATA
segment	.data
; ALIGN
align	4
; LABEL z
z:
; SINT 30
	dd	30
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
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; ADDR y
	push	dword $y
; LDINT
	pop	eax
	push	dword [eax]
; NE
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setne	cl
	mov	[esp], ecx
; DUP32
	push	dword [esp]
; JNZ _L1
	pop	eax
	cmp	eax, byte 0
	jne	near _L1
; INT 7
	push	dword 7
; DUP32
	push	dword [esp]
; ADDR z
	push	dword $z
; STINT
	pop	ecx
	pop	eax
	mov	[ecx], eax
; OR
	pop	eax
	or	dword [esp], eax
; ALIGN
align	4
; LABEL _L1
_L1:
; TRASH 4
	add	esp, 4
; ADDR z
	push	dword $z
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL _L2
_L2:
; LABEL _L3
_L3:
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
