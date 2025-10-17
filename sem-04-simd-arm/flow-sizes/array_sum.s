.section .rodata
size:
    .quad 3
array:
    .int 1
    .int -2
    .int 3

    .text
    .global array_sum

    .extern sum_result

array_sum:
    adr x0, array
    adr x1, size
    ldr x1, [x1]

    // accumulated sum
    mov x2, 0

    // for x3 = 0 to x1
    mov x3, 0
loop:
    cmp x3, x1
    bhs loop_end

    // uncomment line 44 or switch 29-30 to 34-35 to fix handling negative result
    ldr w4, [x0, x3, lsl 2]
    add w2, w2, w4
    
    // load signed extended word-sized memory
    // sizes: word(w), half-word(h), byte(b)
    // ldrsw x4, [x0, x3, lsl 2]
    // add x2, x2, x4

    add x3, x3, 1
    b loop
loop_end:

    adr x4, sum_result

    // signed extend of word(4-bytes)
    // sxtw x2, w2
    str x2, [x4]
    ret
