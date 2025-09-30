; Vector Triad x86-64 assembly implementation

section .text
bits 64
default rel
global vector_triad_x86_64

vector_triad_x86_64:
	push rbp
	mov rbp, rsp
	add rbp, 16
	
	xor rax, rax
	mov rax, [rbp+32]
	
	pop rbp
L1:
	; transfer address of arrays to registers
	vmovss xmm1, [r8+rcx*4]
	vmovss xmm2, [r9]
	vmovss xmm3, [rax]

	vmulss xmm2, xmm2, xmm3
	vaddss xmm0, xmm1, xmm2
	vmovss [rdx], xmm0

	add r8, 4
	add r9, 4
	add rax, 4
	add rdx, 4
	
	LOOP L1
finis:
	ret