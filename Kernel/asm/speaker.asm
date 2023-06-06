GLOBAL setPITFrq
GLOBAL playSound
GLOBAL stopSound

section .text

%macro getTmp 0
    mov rax, 0
    in al, 0x61     ; Estado del Speaker, entre otras cosas.
%endmacro

setPITFrq:
    push rbp
    mov rbp, rsp

    push rdx        ; Se debe preservar.

    mov edx, 0      ; Saco por si hay basura. (Esto lo hago porque estoy usando el operando DIV con un registro de 32 bits)
    mov eax, 1193180
    div edi         ; en RDI tengo el valor en 64 bits, por lo tanto tomo solo EDI (32 bits)

    push rax
    mov al, 0xB6
    out 43h, al  ; outb(0x43, 0xb6);
    pop rax
    out 42h, al     ; outb(0x42, (uint8_t) (Div) );
    mov al, ah
    out 42h, al     ; outb(0x42, (uint8_t) (Div >> 8));

    pop rdx

    leave
    ret


; C alike function: void playSound(uint32_t freq)
playSound:
    push rbp
    mov rbp, rsp

    call setPITFrq ; Ya tengo en en RDI el valor esperado. Por lo tanto no hace falta pasarlo

    getTmp
    mov ah, al
    or al, 0x03
    cmp al, ah
    je exit
    out 0x61, al

exit:
    leave
    ret

; detiene la reproduccion del sondido
stopSound:
    push rbp
    mov rbp, rsp

    getTmp 
    and al, 0xFC
    out 0x61, al

    leave
    ret