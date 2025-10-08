; Vector Triad SIMD AVX2 assembly language using XMM register

section .text
bits 64
default rel
global vectriad_SIMDx

vectriad_SIMDx:
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

    ; divide rcx by 4
    shr rcx, 2

L1:
    ; transfer address of arrays to registers
    vmovdqu xmm1, [r8+r10*4]
    vmovdqu xmm2, [r9+r10*4]
    vmovdqu xmm3, [rax+r10*4]

    ; perform vector triad operation
    vmulps xmm0, xmm2, xmm3
    vaddps xmm0, xmm0, xmm1
    vmovdqu [rdx+r10*4], xmm0

    add r10, 4
    loop L1

    ; handle remaining elements
    mov rcx, r11
    and rcx, 3
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