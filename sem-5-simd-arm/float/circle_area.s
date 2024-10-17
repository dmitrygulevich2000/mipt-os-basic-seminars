    .intel_syntax noprefix

    .section .rodata
pi:
    .float 3.14159265358979323846

    .text
    .global circle_area

circle_area:
    push rbp
    mov rbp, rsp
    // float arguments in xmm0, xmm1, ..., xmm7

    // scalar, single-precision
    // xmm0 *= xmm0
    mulss xmm0, xmm0
    movss xmm1, pi
    mulss xmm0, xmm1

    // float return value in xmm 0
    mov rsp, rbp
    pop rbp
    ret
