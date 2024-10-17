#include <sys/syscall.h>

    .text
    .global _start

_start:
    // write(1, message, 14)
    // syscall number in : x8
    mov x8, SYS_write
    mov x0, 1
    adr x1, message
    mov x2, 14
    svc 0
    
    // exit(0)
    mov x8, SYS_exit
    mov x0, 0
    svc 0

    .section .rodata
message:
    .string "Hello, world!\n"
