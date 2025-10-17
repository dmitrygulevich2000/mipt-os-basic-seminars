    .intel_syntax noprefix

    .global prepare_rsp_asm
    .global switch_to_and_save_current
    .global prepare_rsp_asm_simple

# void* prepare_rsp_asm(void* stack, void* helper, void* helper_arg)
prepare_rsp_asm:
    mov r10, rsp
    mov rsp, rdi

    # -16 = 0b1..10000
    and rsp, -16
    
    mov [rsp], rdx
    sub rsp, 8
    push rsi

    push 0
    push 0
    push 0
    push 0
    push 0
    push 0

    mov rax, rsp

    mov rsp, r10

    ret

# void switch_to_and_save_current(void **rsp_for_save, void* to_rsp)
switch_to_and_save_current:
    push rbp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov [rdi], rsp

    mov rsp, rsi

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp

    ret
