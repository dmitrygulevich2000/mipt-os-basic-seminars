    .intel_syntax noprefix

    .text
    .global func

func:
    mov rdi, [rip + arg1]
    mov rsi, [rip + arg2]
    mov qword ptr [rip + result], 100

    cmp rdi, 10
    ja do
    cmp rsi, 20
    jnz next
do:
    add qword ptr [rip + result], 1
next:
 
    ret
