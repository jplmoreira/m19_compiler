; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
        ;; before body 
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL _L1
_L1:
; LABEL _L2
_L2:
        ;; after body 
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
; INT 3
	push	dword 3
; CALL f
	call	f
; TRASH 4
	add	esp, 4
; TRASH 0
	add	esp, 0
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
; EXTERN printi
extern	printi
