GLOBAL getChar
GLOBAL putChar
GLOBAL putPixel
GLOBAL updateScreen
GLOBAL setBuffer
GLOBAL getKeyState
GLOBAL setForegroundColour
GLOBAL setBackgroundColour
GLOBAL clearScreen
GLOBAL showRegisters
GLOBAL setPrintAnywhere
GLOBAL setFontSize
GLOBAL mem_initialize
GLOBAL malloc
GLOBAL free

section .text
; realiza la llamada a la syscall de read
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

; realiza la llamada a la syscall de write
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
; realiza la syscall a la syscall para imprimir un pixel en pantalla
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
; llamado a la syscall de actualizar la pantalla
updateScreen:
    push rbp
    mov rbp, rsp

    mov rdi, 9
    int 80h

    leave
    ret
; llamado a la sycall para setear el doble buffer para suavizar el cambio de pantalla
setBuffer:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 3
    int 80h

    leave
    ret
; retorna el estado de la key, si esta apretada o no
getKeyState:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 12
    int 80h
    
    mov rax, rsi
    leave
    ret
; llamado de la syscall para cambiar el color de letra
setForegroundColour:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 10
    int 80h

    leave
    ret
; llamado a la syscall para cambiar el fondo de la letra
setBackgroundColour:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 11
    int 80h

    leave
    ret
; llamado a la sycall para limpiar la pantalla
clearScreen:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 2
    int 80h

    leave
    ret
; llamado a la syscall para mostrar los registros
showRegisters:
     push rbp
     mov rbp, rsp

     mov rdi, 13
     int 80h

     leave
     ret
; llamado a la syscall para poder setear el lugar de impresion en pantalla
setPrintAnywhere:
   push rbp
   mov rbp, rsp

   mov r10, rdi
   mov r9,  rsi

   mov rdi, 14
   int 80h

   leave
   ret

setFontSize:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 4 ; syscall 4
    int 80h

    leave
    ret

mem_initialize:
    push rbp
    mov rbp, rsp

    mov rdi, 17 ; syscall 17
    int 80h

    leave
    ret

malloc:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 18 ; syscall 18
    int 80h

    leave
    ret

free:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 19 ; syscall 19
    int 80h

    leave
    ret

section .bss
placeholder resb 1