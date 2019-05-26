; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 8
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; LOCA -4
	pop	eax
	mov	[ebp-4], eax
        ;; before body 
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; DUP64
; SP
	push	esp
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LABEL _L1
_L1:
; LABEL _L2
_L2:
        ;; after body 
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; LDDOUBLE
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; STFVAL64
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
	ret
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
; INT 10
	push	dword 10
; CALL f
	call	f
; TRASH 4
	add	esp, 4
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; INT 2
	push	dword 2
; CALL f
	call	f
; TRASH 4
	add	esp, 4
; LDFVAL64
	sub	esp, byte 8
	fstp	qword [esp]
; CALL printd
	call	printd
; TRASH 8
	add	esp, 8
; LABEL _L3
_L3:
; LABEL _L4
_L4:
        ;; after body 
; LOCV -4
	push	dword [ebp-4]
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printd
extern	printd
