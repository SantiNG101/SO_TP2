GLOBAL clearScreen

EXTERN puts

section .text

clearScreen:
    ; Llama a la sys de setTerminal
    mov rdi, 2
    INT 80h

    ; mov rdi, cadena
    ; call puts

    ret

; section .data
; cadena db "$ ", 0