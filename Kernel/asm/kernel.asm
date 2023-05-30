
GLOBAL kernelASM

section .text
kernelASM: 
    ; mov rdi, 0
    ; mov rsi, 1
    ; mov r10, cadena
    ; mov r9, longitude
    ; int 80h

    ; mov rdi, 1
    ; mov r10, cadena
    ; mov r9, longitude
    ; int 80h

    ; mov rdi, 0
    ; mov r10, cadena
    ; mov r9, longitude
    ; int 80h

    mov cr6, rax
    ret

section .data
cadena db "Escriba algo: ", 0
longitude equ $-cadena

section .bss
placeholder resb 140