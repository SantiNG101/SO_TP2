GLOBAL getChar
GLOBAL putChar

section .text
getChar:
    push rbp
    mov rbp, rsp

    mov rdi, 1
    mov r10, placeholder
    mov r9, 1
    int 80h

    mov rdi, [placeholder]
    call putChar
    
    mov rax, 0
    mov rax, [placeholder]

    leave
    ret

putChar:
    push rbp
    mov rbp, rsp

    mov [placeholder], rdi  ; Por rdi viene el caracter a imprimir.

    mov r10, placeholder   
    mov r9, 1               ; Imprimo solo el caracter.
    mov rdi, 0              ; syswrite.
    int 80h

    mov rax, rdi            ; retorno el caracter impreso.
    leave
    ret

section .bss
placeholder resb 1