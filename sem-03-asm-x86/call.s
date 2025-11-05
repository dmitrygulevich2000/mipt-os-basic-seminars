    .intel_syntax noprefix

    .text
    .global main

greet:
    // must preserve : rbx, rsp, rbp, r12, r13, r14, r15 (callee-saved)
    // standard prologue
    push rbp
    mov rbp, rsp
    push r12

    // char str[56]
    sub rsp, 56

    add r12, 1

    // scanf("%d", &str)
    // arguments passed in : rdi, rsi, rdx, rcx, r8, r9
    lea rdi, [rip + in_fmt]
    mov rsi, rsp
    // can change : rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11 (caller-saved)
    call scanf
    // return value in : rax

    lea rdi, out_fmt[rip]
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

    mov r12, 0

    call greet

    mov rsp, rbp
    pop rbp

    mov rax, r12
    ret

    .section .rodata
in_fmt:
    .string "%s"
out_fmt:
    .string "Hello, %s!\n"
