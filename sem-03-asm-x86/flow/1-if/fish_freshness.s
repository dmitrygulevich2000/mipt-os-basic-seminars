    .intel_syntax noprefix

    .text
    .global fish_freshness

    .extern days

fish_freshness:
    mov rdi, [rip + days]

    cmp rdi, 3
    jl else
    mov qword ptr [rip + freshness], 2
    jmp next
else:
    mov qword ptr [rip + freshness], 1
next:
  
    ret
