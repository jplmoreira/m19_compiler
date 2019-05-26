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
; ADDR y
	push	dword $y
; LDINT
	pop	eax
	push	dword [eax]
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; GT
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setg	cl
	mov	[esp], ecx
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL _L1
_L1:
; LABEL _L2
_L2:
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
