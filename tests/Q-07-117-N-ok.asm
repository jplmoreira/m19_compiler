; TEXT
segment	.text
; ALIGN
align	4
; LABEL main
main:
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
; LDINT
	pop	eax
	push	dword [eax]
; INT 0
	push	dword 0
