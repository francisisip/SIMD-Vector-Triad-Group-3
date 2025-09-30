; Vector Triad SIMD AVX2 assembly language using YMM register

section .text
bits 64
default rel
global vectriad_SIMDy

vectriad_SIMDy:
    ; clear out registers
	xor rbx, rbx
	xor rsi, rsi

	; obtain 5th parameter from stack
	push rbp
	mov rbp, rsp
	add rbp, 16
	mov rbx, [rbp+32]
	pop rbp

    ; divide rcx by 8 with boundary condition
    add rcx, 7
    shr rcx, 3

L1:
    ; transfer address of arrays to registers
    vmovdqu ymm1, [r8+rsi*4]
    vmovdqu ymm2, [r9+rsi*4]
    vmovdqu ymm3, [rbx+rsi*4]

    ; perform vector triad operation
    vmulps ymm0, ymm2, ymm3
    vaddps ymm0, ymm0, ymm1
    vmovdqu [rdx+rsi*4], ymm0
    
    inc rsi
    loop L1

    ret