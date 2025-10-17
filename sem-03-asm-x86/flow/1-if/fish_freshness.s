    .intel_syntax noprefix

    .text
    .global fish_freshness

    .extern days

fish_freshness:
    mov rdi, days[rip]

    cmp rdi, 3
    jl else
    mov qword ptr freshness[rip], 2
    jmp next
else:
    mov qword ptr freshness[rip], 1
next:
  
    ret
