GLOBAL terminalSetter
GLOBAL pongUpdater

EXTERN puts

section .text
; llama a la syscall para setear la terminal
terminalSetter:    
    mov rdi,2
    INT 80h
    ret
    