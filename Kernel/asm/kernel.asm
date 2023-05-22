
GLOBAL kernelASM

EXTERN putChar
EXTERN ncNewline

section .text
kernelASM: 
    mov rdi, 0
    mov rsi, 1
    mov r10, cadena
    mov r9, longitude
    int 80h

    ;  mov rdi, 1
    ;  mov r10, placeholder
    ;  mov r9, 4
    ; int 80h
    ; A PARTIR DE ACÁ COMIENZAN LOS PROBLEMAS
    mov rcx, 4
    mov rdi, placeholder
    again: 
        call putChar
        inc rdi
        dec rcx
        cmp rcx, 0
        jne again

    call ncNewline

    mov rdi, 0
    mov r10, placeholder
    mov r9,  4
    int 80h

    ret

section .data
cadena db "Ingrese un decimal: ", 0
longitude equ $-cadena
cadena2 db "santi puto: ",0
longitude2 equ $-cadena2

section .bss
placeholder resb longitude