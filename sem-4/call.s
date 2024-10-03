    .intel_syntax noprefix

    .text
    .global main

greet:
    // must preserve : rbx, rsp, rbp, r12, r13, r14, r15
    // standard prologue
    push rbp
    mov rbp, rsp

    sub rsp, 64

    // arguments passed in : rdi, rsi, rdx, rcx, r8, r9
    mov rdi, offset in_fmt
    mov rsi, rsp
    // can change : rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11
    call scanf
    // return value in : rax

    mov rdi, offset out_fmt
    mov rsi, rsp
    call printf

    mov rax, 0

    // standard epilogue
    mov rsp, rbp
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp

    call greet

    mov rsp, rbp
    pop rbp
    ret

    .section .rodata
in_fmt:
    .string "%s"
out_fmt:
    .string "Hello, %s!\n"
