GLOBAL getChar

EXTERN putChar

section .text
getChar:
    push rbp
    mov rbp, rsp

    mov rdi, 1
    mov r10, placeholder
    mov r9, 1
    INT 80h

    mov rdi, [placeholder]
    call putChar
    
    mov rax, 0
    mov rax, [placeholder]

    leave
    ret

section .bss
placeholder resb 1