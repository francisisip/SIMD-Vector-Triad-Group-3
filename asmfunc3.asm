; Vector Triad SIMD AVX2 assembly language using YMM register

section .text
bits 64
default rel
global vectriad_SIMDy

vectriad_SIMDy:
    ; clear out registers
	xor rax, rax
	xor r10, r10
    xor r11, r11

	; obtain 5th parameter from stack
	push rbp
	mov rbp, rsp
	add rbp, 16
	mov rax, [rbp+32]
	pop rbp

    ; save rcx (number of elements)
    mov r11, rcx

    ; divide rcx by 8
    shr rcx, 3

L1:
    ; transfer address of arrays to registers
    vmovdqu ymm1, [r8+r10*4]
    vmovdqu ymm2, [r9+r10*4]
    vmovdqu ymm3, [rax+r10*4]

    ; perform vector triad operation
    vmulps ymm0, ymm2, ymm3
    vaddps ymm0, ymm0, ymm1
    vmovdqu [rdx+r10*4], ymm0
    
    add r10, 8
    loop L1

    ; handle remaining elements
    mov rcx, r11
    and rcx, 7
    test rcx, rcx
    jz finis

L2:
    ; transfer address of arrays to registers
    vmovss xmm1, [r8+r10*4]
    vmovss xmm2, [r9+r10*4]
    vmovss xmm3, [rax+r10*4]

    ; perform vector triad operation
    vmulss xmm0, xmm2, xmm3
    vaddss xmm0, xmm0, xmm1
    vmovss [rdx+r10*4], xmm0

    inc r10
    loop L2

finis:
    ret