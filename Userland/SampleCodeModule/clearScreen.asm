GLOBAL clearScreen

EXTERN puts

section .text

clearScreen:
    ; Llama a la sys de setTerminal
    mov rdi, 2
    INT 80h

    ret