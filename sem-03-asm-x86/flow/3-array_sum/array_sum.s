    // directive
    .intel_syntax noprefix

    // read-only data
    .section .rodata
size:
    // quad-word <=> 4 * 2-byte
    .quad 4
array:
    .int 1
    .int 2
    .int 30
    .int 3

    .text
    .global array_sum

    // not required
    .extern sum_result

array_sum:
    mov eax, 0
    lea rdi, array[rip]
    mov rsi, size[rip]

    // for rcx = 0 to rsi
    mov rcx, 0
loop:
    cmp rcx, rsi
    jge loop_end

    mov edx, [rdi + 4*rcx + 0]
    add eax, edx

    inc rcx
    jmp loop
loop_end:

    mov sum_result[rip], eax
    ret
