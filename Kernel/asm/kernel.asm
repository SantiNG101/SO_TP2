
GLOBAL kernelASM

section .text

kernelASM: 
    ; mov rip, rdi
    ret

section .data
cadena db "Escriba algo: ", 0
longitude equ $-cadena

section .bss
placeholder resb 140