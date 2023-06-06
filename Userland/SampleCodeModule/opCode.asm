GLOBAL opCode

section .text
; realiza la excepcion de operacion invalida
opCode:
     mov cr6, rax
     ret