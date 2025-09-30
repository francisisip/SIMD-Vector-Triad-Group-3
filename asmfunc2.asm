; Vector Triad SIMD AVX2 assembly language using XMM register

section .text
bits 64
default rel
global vectriad_SIMDx

vectriad_SIMDx:
    ; clear out registers
	xor rbx, rbx
	xor rsi, rsi

	; obtain 5th parameter from stack
	push rbp
	mov rbp, rsp
	add rbp, 16
	mov rbx, [rbp+32]
	pop rbp

    ; divide rcx by 4 with boundary condition
    add rcx, 3
    shr rcx, 2

L1:
    ; transfer address of arrays to registers
    vmovdqu xmm1, [r8+rsi*4]
    vmovdqu xmm2, [r9+rsi*4]
    vmovdqu xmm3, [rbx+rsi*4]

    ; perform vector triad operation
    vmulps xmm0, xmm2, xmm3
    vaddps xmm0, xmm0, xmm1
    vmovdqu [rdx+rsi*4], xmm0

    inc rsi
    loop L1

    ret