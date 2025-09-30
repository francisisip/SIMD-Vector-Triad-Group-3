section .text
bits 64
default rel
global vecaddSIMDy

vecaddSIMDy:
    push rbp
	mov rbp, rsp
	add rbp, 16
	
	xor rax, rax
	mov rax, [rbp+32]
	
	pop rbp

    add rcx, 7 ; boundary condition
    shr rcx, 3 ; divide it by 8

L1:
    vmovdqu ymm1, [r8]
    vmovdqu ymm2, [r9]
    vmovdqu ymm3, [rax]
    vmulps ymm0, ymm2, ymm3
    vaddps ymm0, ymm0, ymm1
    vmovdqu [rdx], ymm0
    add r8, 32           
    add r9, 32
    add rax, 32
    add rdx, 32
    loop L1               

    ret