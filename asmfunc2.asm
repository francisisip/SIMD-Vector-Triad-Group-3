section .text
bits 64
default rel
global vectriad_SIMDx

vectriad_SIMDx:
    push rbp
	mov rbp, rsp
	add rbp, 16
	
	xor rax, rax
	mov rax, [rbp+32]
	
	pop rbp

    add rcx, 3 ; boundary condition
    shr rcx, 2 ; divide it by 4

L1:
    vmovdqu xmm1, [r8]
    vmovdqu xmm2, [r9]
    vmovdqu xmm3, [rax]
    vmulps xmm0, xmm2, xmm3
    vaddps xmm0, xmm0, xmm1
    vmovdqu [rdx], xmm0
    add r8, 16            
    add r9, 16
    add rax, 16
    add rdx, 16
    loop L1               

    ret