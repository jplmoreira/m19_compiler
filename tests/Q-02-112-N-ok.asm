; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL main, :function
global	main:function
; LABEL main
main:
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
; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 12
	push	ebp
	mov	ebp, esp
	sub	esp, 12
; INT 0
	push	dword 0
; LOCA -4
	pop	eax
	mov	[ebp-4], eax
        ;; before body 
