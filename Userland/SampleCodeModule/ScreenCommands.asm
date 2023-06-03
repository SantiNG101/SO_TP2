GLOBAL screenSetter
GLOBAL clearScreen

EXTERN puts

section .text

screenSetter:
    ; Llama a la sys de setter
    mov r10,rdi
    mov rdi, 2
    INT 80h
    ret
clearScreen:
    ;setea el modo terminal para limpiar la pantalla
    mov r10, 0
    mov rdi, 2
    INT 80h
    ret