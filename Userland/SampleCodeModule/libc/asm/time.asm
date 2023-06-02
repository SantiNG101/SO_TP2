GLOBAL sleep
GLOBAL timestamp
GLOBAL ticks

section .text

; C alike function void sleep(uint64_t ms);
sleep:
    push rbp
    mov rbp, rsp

    mov r10, rdi
    mov rdi, 5
    INT 80h

    leave
    ret

timestamp:
    mov rdi, 7          ; Llamada a la syscall de timeNow
    INT 80h

    mov rax, r10
    ret

ticks:
    ret