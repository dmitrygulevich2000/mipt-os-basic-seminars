    .intel_syntax noprefix

    .text
    .global func

func:
    mov rdi, arg1[rip]
    mov rsi, arg2[rip]
    mov qword ptr result[rip], 100

    cmp rdi, 10
    ja do
    cmp rsi, 20
    jnz next
do:
    add qword ptr result[rip], 1
next:
 
    ret
