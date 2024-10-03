#include <sys/syscall.h>
    
    .intel_syntax noprefix

    .text
    .global _start

_start:
    // write(1, message, 14)
    // syscall number in : rax
    mov rax, SYS_write
    mov rdi, 1
    mov rsi, offset message
    mov rdx, 14
    syscall
    
    // ret
    
    // exit(0)
    mov rax, SYS_exit
    mov rdi, 1
    syscall

    .section .rodata
message:
    .string "Hello, world!\n"
