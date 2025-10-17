    .intel_syntax noprefix

    .text
    .global main

greet:
    // must preserve : rbx, rsp, rbp, r12, r13, r14, r15 (callee-saved)
    // standard prologue
    push rbp
    mov rbp, rsp
    push r12

    sub rsp, 56

    // modify r12
    // ...

    // arguments passed in : rdi, rsi, rdx, rcx, r8, r9
    lea rdi, [rip + in_fmt]
    mov rsi, rsp
    // can change : rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11 (caller-saved)
    call scanf
    // return value in : rax

    lea rdi, [rip + out_fmt]
    mov rsi, rsp
    call printf

    mov rax, 0

    // standard epilogue
    lea rsp, [rbp - 8]
    pop r12
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp

    call greet

    mov rsp, rbp
    pop rbp

    mov rax, 0
    ret

    .section .rodata
in_fmt:
    .string "%s"
out_fmt:
    .string "Hello, %s!\n"
