GLOBAL playSound
GLOBAL startSound
GLOBAL stopSound

section .text

; C function alike: void playSound(uint64_t frq, uint64_t time)
playSound:
    mov r10, rdi
    mov r9, rsi
    mov rdi, 6
    INT 80h

    ret

startSound:
    mov r10, rdi
    mov rdi, 15
    INT 80h

    ret
    
stopSound:
    mov rdi, 16
    INT 80h

    ret