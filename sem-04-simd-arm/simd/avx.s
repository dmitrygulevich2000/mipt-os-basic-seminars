    .intel_syntax noprefix

    .text
    .global array_avg_avx
    .global array_avg_avx_aligned

array_avg_avx:
    vxorps ymm0, ymm0, ymm0
    
    mov rax, 0
loop8:
    cmp rax, rsi
    jge loop8end

    vmovups ymm1, [rdi + 4*rax]
    // vector ...
    // ymm0 = ymm0 + ymm1
    vaddps ymm0, ymm0, ymm1
    
    add rax, 8
    jmp loop8
loop8end:

    vextractf128 xmm1, ymm0, 1
    haddps xmm0, xmm1
    haddps xmm0, xmm0
    haddps xmm0, xmm0

    cvtsi2ss xmm1, rsi
    divss xmm0, xmm1
    ret

array_avg_avx_aligned:
    vxorps ymm0, ymm0, ymm0
    
    mov rax, 0
aligned_loop8:
    cmp rax, rsi
    jge aligned_loop8end

    // aligned, ...
    vmovaps ymm1, [rdi + 4*rax]
    vaddps ymm0, ymm0, ymm1
    
    add rax, 8
    jmp aligned_loop8
aligned_loop8end:

    vextractf128 xmm1, ymm0, 1
    haddps xmm0, xmm1
    haddps xmm0, xmm0
    haddps xmm0, xmm0

    cvtsi2ss xmm1, rsi
    divss xmm0, xmm1
    ret
