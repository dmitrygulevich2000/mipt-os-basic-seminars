    .text
    .global main
    
    .macro push Xn:req
        sub sp, sp, 8
        str \Xn, [sp]
        // or equivalently:
        // str \Xn, [sp, -8]!
        // see pre-index
    .endm

    .macro pop Xn:req
        ldr \Xn, [sp]
        add sp, sp, 8
        // or equivalently:
        // ldr \Xn, [sp], 8
        // see post-index
    .endm

greet:
    // must preserve : x19 - x28, x29-x31
    // prologue
    push fp // x29
    push lr // x30
    mov fp, sp

    sub sp, sp, 64

    // arguments passed in : x0-x7
    adr x0, in_fmt
    mov x1, sp
    // can change : x0-x7, x9-x15
    bl scanf
    // return value in : x0

    adr x0, out_fmt
    mov x1, sp
    bl printf

    mov x0, 0

    // epilogue
    mov sp, fp
    pop lr
    pop fp
    // jumps to lr
    ret

main:
    push lr

    // saves return address to lr
    bl greet

    pop lr
    ret

    .section .rodata
in_fmt:
    .string "%s"
out_fmt:
    .string "Hello, %s!\n"
