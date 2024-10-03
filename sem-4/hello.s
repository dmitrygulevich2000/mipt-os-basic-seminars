    .intel_syntax noprefix

    .text
    .global main

main:
    push rbp
    mov rbp, rsp

    mov rdi, offset message
    call printf

    mov rax, 0
    
    pop rbp
    ret

    .section .rodata
message:
    .string "Hello, world!\n"
