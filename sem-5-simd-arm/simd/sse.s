    .intel_syntax noprefix

    .text
    .global array_avg_sse

array_avg_sse:
    // packed, single precision
    xorps xmm0, xmm0
  
    mov rax, 0
// per 4 array elem
loop4:
    cmp rax, rsi
    jge loop4end
  
    // unaligned, packed, single
    // [array + sizeof(float)*index]
    movups xmm1, [rdi + 4*rax]
    addps xmm0, xmm1
  
    // index += 4
    add rax, 4
    jmp loop4
loop4end:

    haddps xmm0, xmm0
    haddps xmm0, xmm0

    // convert signed integer to scalar single
    cvtsi2ss xmm1, rsi
    divss xmm0, xmm1
    ret
