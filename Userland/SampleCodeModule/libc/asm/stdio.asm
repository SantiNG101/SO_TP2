GLOBAL getChar
GLOBAL putChar
GLOBAL putPixel
GLOBAL updateScreen
GLOBAL setBuffer
GLOBAL getKeyState
GLOBAL setForegroundColour
GLOBAL setBackgroundColour

section .text
getChar:
    push rbp
    mov rbp, rsp

    mov rdi, 1
    mov r10, placeholder
    mov r9, 1
    int 80h
    
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

putPixel:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov r9, rsi
    mov r8, rdx
    mov rdi, 8
    int 80h

    leave
    ret

updateScreen:
    push rbp
    mov rbp, rsp

    mov rdi, 9
    int 80h

    leave
    ret
setBuffer:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 3
    int 80h

    leave
    ret

getKeyState:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 12
    int 80h
    
    mov rax, rsi
    leave
    ret

setForegroundColour:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 10
    int 80h

    leave
    ret

setBackgroundColour:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 11
    int 80h

    leave
    ret

section .bss
placeholder resb 1