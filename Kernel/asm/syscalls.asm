

section .text

GLOBAL syscallHandler

;EXTERN syscallDispactcher

syscallHandler:
    push rbp
    mov rbp, rsp
    ; asigno todos los registros al espacio reservado

    ; pusheo todos los registros como struct

    ;call syscallDispactcher

    ; popeo el struct

    mov rsp, rbp
    pop rbp
    iretq



section .bss
struct resb 768 ; preparo el espacio para guardar y 
                ; formar tod los registros todos los punteros de 64 bits