GLOBAL terminalSetter
GLOBAL pongUpdater

EXTERN puts

section .text
    
terminalSetter:    
    mov rdi,2
    INT 80h
    ret

pongUpdater:
    ; Llama a la sys para actualizar la pantalla del pong
    ; faltan agregarles todos los parametros de punteros que le quieran agregar
    mov r10, rdi
    mov rdi, 4
    INT 80h
    ret
clearScreen:
    ;setea el modo terminal para limpiar la pantalla
    mov rdi, 8
    INT 80h
    ret