GLOBAL getChar
GLOBAL putChar
GLOBAL setTerminal
GLOBAL callWrongExpresionException

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

    mov rax, [placeholder]            ; retorno el caracter impreso.
    leave
    ret

setTerminal:
    push rbp
    mov rbp, rsp
    mov rdi, 2              ; modo terminal.
    int 80h
    leave
    ret

section .bss
placeholder resb 1