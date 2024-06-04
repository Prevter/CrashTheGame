section .text
global corruptTheStack ; void corruptTheStack(void)
global jumpToPointer   ; void jumpToPointer(uint64_t ptr)

corruptTheStack:
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    ret

jumpToPointer:
    mov rax, rdi
    jmp rax

