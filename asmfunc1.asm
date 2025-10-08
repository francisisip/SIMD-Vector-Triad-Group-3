; Vector Triad x86-64 assembly implementation

section .text
bits 64
default rel
global vectriad_x86_64

vectriad_x86_64:
	; clear out registers
	xor rax, rax
	xor r10, r10

	; obtain 5th parameter from stack
	push rbp
	mov rbp, rsp
	add rbp, 16
	mov rax, [rbp+32]
	pop rbp

L1:
	; transfer address of arrays to registers
	vmovss xmm1, [r8+r10*4]
	vmovss xmm2, [r9+r10*4]
	vmovss xmm3, [rax+r10*4]

	; perform vector triad operation
	vmulss xmm0, xmm2, xmm3
	vaddss xmm0, xmm0, xmm1
	vmovss [rdx+r10*4], xmm0
	
	inc r10
	loop L1

finis:
	ret