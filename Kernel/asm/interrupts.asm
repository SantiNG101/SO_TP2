; Parte del código es ofrecido por la cátedra y se da como ejemplo para armar
; las distintas posibles interrupciones.


; ACERCA DEL DOCUMENTO:
    ;
    ; El siguiente documento se encarga de armar la lista de handlers
    ; para las distintas interrupciones del sistema.
    ; 
    ; Parte del codigo es ofrecido por la catedra y se da como ejemplo.
    ; Como que nos gusto la forma en que la catedra resolvio la 
    ; insercion de nuevas interrupciones, decidimos utilizar
    ; un mecanismo similar, con sutiles variaciones que afectan
    ; la velocidad y el espacio de memoria ocupado en RAM por la KERNEL.
    ;
    ; Dichos cambios no se encuentran propiamente en este archivo,
    ; pero si en los de exceptions.c e irq.c, donde se puede evidenciar
    ; como cambiamos el switch por un arreglo de funciones.
    ;
    ; Si bien, este arreglo ocuparia mas espacio de memoria evita
    ; comparaciones y el mantenimiento promete ser menos laborioso.
    ;
    ; Por el resto, el codigo se explica por si mismo.
    ;

; ------ GLOBALS  SECTION ------ ;
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL prepare_process

GLOBAL _irq00Handler        ;   TIMER TICK
GLOBAL _irq01Handler        ;   KEYBOARD
GLOBAL _irq02Handler        ;   SYSCALL

GLOBAL _exception00Handler  ;   Division by Zero Exception
GLOBAL _exception01Handler  ;   Not an available function
GLOBAL _exception02Handler  ;   Default exception

; ------ ENDS GLB SECTION ------ ;

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN getStackBase
EXTERN schedule

section .text

; ------ START MACROS ------ ;

%macro pushState 0
    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
    push rbp
    mov rbp, rsp            ; Armo el stack frame

    pushState
    
    mov rdi, %1                 ; Paso el parametro
    mov rsi, rsp                ; Paso la estructura del estado
    mov rcx, [rbp + 32]         ; Paso el rsp viejo
    call irqDispatcher          ; Ejecuto la interrupción correspondiente con irqDispatcher

    mov al, 20h                 ; Signal PIC EOI (End of Interrupt)
    out 20h, al

    popState

    mov rsp, rbp            ; Desarmo el StackFrame
    pop rbp
    iretq
%endmacro

%macro exceptionHandlerMaster 1
    push rbp
    mov rbp, rsp                ; Armo el stackFrame

    pushState

    mov rdi, %1                 ; Paso el parametro
    mov rsi, [rbp + 8]
    mov rdx, rsp                ; Paso la estructura del estado.
    ; Tengo que pasar el RSP Viejo.
    mov rcx, [rbp + 32]
    call exceptionDispatcher    ; Ejecuto la excepción correspondiente
    mov [rbp + 8], rax          ; Cambio el RIP

    call getStackBase           ; Obtengo la base del stack
    sub rax, 40
    mov [rbp + 32], rax         ; Cambio el Original RSP

    popState

    mov rsp, rbp                ; Desarmo el stackFrame
    pop rbp
    iretq
%endmacro

; ------ END MACROS ------ ;

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

get_ip:
    mov rax, [rbp + 8]
    ret

prepare_process:
    push rbp
    mov rbp, rsp
    mov rsp, rdi

    push dword 0x0      ;SS
    push rdi            ;RSP
    push qword 0x202    ;RFLAGS
    push dword 0x8      ;CS
    push rsi            ;RIP
    push qword 0x14     ;R15
    push qword 0x13     ;R14
    push qword 0x12     ;R13
    push qword 0x11     ;R12
    push qword 0x10     ;R11
    push qword 0x9      ;R10
    push qword 0x8      ;R9
    push qword 0x7      ;R8
    push rcx            ;RSI
    push rdx            ;RDI
    push rdi            ;RBP
    push qword 0x3      ;RDX
    push qword 0x2      ;RCX
    push qword 0x1      ;RBX
    push qword 0x0      ;RAX

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret

; ------ STARTS IRQ ------ ;

_irq00Handler:; TIMER TICK
    pushState
    cli
	mov rdi, 0 ; pasaje de parametro
    mov rsi, rsp                ; Paso la estructura del estado
    mov rcx, [rbp + 32] 
	call irqDispatcher

	mov rdi, rsp
	call schedule
	mov rsp, rax

	mov al, 20h
	out 20h, al
    sti
	popState
	iretq

_irq01Handler:
    irqHandlerMaster 1          ; KEYBOARD

_irq02Handler:                  ; SYSCALL
    push rbp
    mov rbp, rsp

    pushf
    ;sti                          Not proud of this, pero necesario si quiero que las syscall funcionen con interrupción de Hardware
    
    pushState                   ; Mejorar para que siga el estándar de C
    mov rdi, rsp
    call syscallDispatcher
    popState

    popf

    mov al, 20h                 ; Signal PIC EOI (End of Interrupt)
    out 20h, al

    mov rsp, rbp
    pop rbp

    iretq


; ------  ENDS  IRQ ------ ;

; ------ STARTS EXCPETIONS ------ ; 

_exception00Handler:
    exceptionHandlerMaster 0    ; Division by Zero Exception

_exception01Handler:
    exceptionHandlerMaster 1    ; Not valid OpCode

_exception02Handler:
    exceptionHandlerMaster 2    ; Default exception

; ------  ENDS  EXCPETIONS ------ ; 

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1