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
GLOBAL execve
GLOBAL getpid
GLOBAL get_pid_parent
GLOBAL set_status
GLOBAL kill_process
GLOBAL show_processes
GLOBAL change_priority
GLOBAL yield
GLOBAL exit_process
GLOBAL create_semaphore
GLOBAL destroy_semaphore
GLOBAL semaphore_wait
GLOBAL semaphore_post
GLOBAL get_fd
GLOBAL pipe_open
GLOBAL pipe_close
GLOBAL set_fd
GLOBAL lower_prio
GLOBAL wait_children
GLOBAL alloc
GLOBAL free_alloc
GLOBAL wait_time

section .text

wait_time:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; quantums
    mov rdi, 40     ; syscall wait
    int 80h

    leave
    ret

alloc:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; size
    mov rdi, 38     ; syscall memalloc
    int 80h

    leave
    ret

free_alloc:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; mem
    mov rdi, 39     ; syscall free
    int 80h

    leave
    ret

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

execve:
    push rbp
    mov rbp, rsp

    mov r9, r8      ; foreGround
    mov r8, rcx     ; argv
    mov rcx, rdx    ; argc
    mov rdx, rsi    ; rip
    mov rsi, rdi    ; parent
    mov rdi, 17     ; execve
    int 80h

    leave
    ret

exit_process:
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, 30  ; exit
    int 80h

    leave
    ret

getpid:
    push rbp
    mov rbp, rsp

    mov rdi, 24  ; get_pid
    int 80h

    leave
    ret

get_pid_parent:
    push rbp
    mov rbp, rsp

    mov rdi, 25  ; get_pid_parent
    int 80h     
    
    leave
    ret

set_status:
    push rbp
    mov rbp, rsp

    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 26  ; set_status
    int 80h

    leave
    ret

kill_process:
    push rbp
    mov rbp, rsp

    mov rsi, rdi
    mov rdi, 28  ; kill_process
    int 80h

    leave
    ret

show_processes:
    push rbp
    mov rbp, rsp

    mov rdi, 23  ; show_processes
    int 80h

    leave
    ret

change_priority:
    push rbp
    mov rbp, rsp

    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 27  ; change_priority
    int 80h

    leave
    ret

yield:
    push rbp
    mov rbp, rsp

    mov rdi, 29 ; yield
    int 80h

    leave
    ret

; crea un nuevo semáforo
create_semaphore:
    push rbp
    mov rbp, rsp

    mov r10, rdi  ; nombre del semáforo
    mov r9,  rsi  ; valor inicial del semáforo
    mov rdi, 18   ; syscall create_semaphore
    int 80h

    leave
    ret

; abre un semáforo existente
open_semaphore:
    push rbp
    mov rbp, rsp

    mov r10, rdi  ; nombre del semáforo
    mov rdi, 19   ; syscall open_semaphore
    int 80h

    leave
    ret

; cierra un semáforo
close_semaphore:
    push rbp
    mov rbp, rsp

    mov r10, rdi  ; puntero al semáforo
    mov rdi, 20   ; syscall close_semaphore
    int 80h

    leave
    ret

; espera en un semáforo
semaphore_wait:
    push rbp
    mov rbp, rsp

    mov r10, rdi    ; puntero al semáforo
    mov rdi, 21     ; syscall semaphore_wait
    int 80h

    leave
    ret

; libera (post) un semáforo
semaphore_post:
    push rbp
    mov rbp, rsp

    mov r10, rdi    ; puntero al semáforo
    mov rdi, 22     ; syscall semaphore_post
    int 80h

    leave
    ret

get_fd:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; mode
    mov rdi, 31     ; syscall get_fd
    int 80h

    leave
    ret

pipe_open:
    push rbp
    mov rbp, rsp

    mov rcx, rdx    ; mode
    mov rdx, rsi    ; id
    mov rsi, rdi    ; pid
    mov rdi, 32     ; syscall open pipe
    int 80h

    leave
    ret

pipe_close:
    push rbp
    mov rbp, rsp

    mov rcx, rdx    ; mode
    mov rdx, rsi    ; id
    mov rsi, rdi    ; pid
    mov rdi, 33     ; syscall close pipe
    int 80h

    leave
    ret

set_fd:
    push rbp
    mov rbp, rsp

    mov rcx, rdx    ; pos
    mov rdx, rsi    ; new fd
    mov rsi, rdi    ; pid
    mov rdi, 34     ; syscall set descriptor
    int 80h

    leave
    ret

lower_prio:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; pid
    mov rdi, 35     ; syscall lower priority
    int 80h

    leave
    ret


wait_children:
    push rbp
    mov rbp, rsp

    mov rsi, rdi    ; pid
    mov rdi, 37     ; syscall lower priority
    int 80h

    leave
    ret

section .bss
placeholder resb 1

