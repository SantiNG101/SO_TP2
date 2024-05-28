GLOBAL cpuVendor
GLOBAL dVideo
GLOBAL clock
GLOBAL read_port
GLOBAL getCurrentRAX
GLOBAL getCurrentRBX
GLOBAL getCurrentRCX
GLOBAL getCurrentRDX
GLOBAL getCurrentRBP
GLOBAL getCurrentRSP
GLOBAL getCurrentRDI
GLOBAL getCurrentRSI
GLOBAL cleanActualRegisters
GLOBAL prepare_process

section .text
;Hay que pasar a parte de ah y al para escribir los bytes con x color
;ah da el color , al va el caracter
dVideo:
	push rbp
	mov rbp, rsp

	push rbx

	mov rbx, rdi			;puntero al current video
	xor eax, eax			;limpio eax
	mov ah, 0x02				;asigno color verde con fondo BLANCO
.print:
	mov al, byte [rsi]		;cargo el string
	cmp al, 0	;veo si termino ya el str
	je .fin
	mov word [rbx], ax		;muevo los primeros 32 bits a rbx 
	add rbx, 2				;video son 2 bytes
	inc rsi					;paso al siguiente caracter
	jmp .print
.fin:
	mov rax, rbx			;retorna el currentVideo en la posicion actual
	
	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; Obtiene el horario de la computadora
clock:
	push rbp
	mov rbp, rsp
	
	mov al, dil
	out 70h, al
	xor rax, rax
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; lee del puerto y lo retorna
read_port:
	push rbp
	mov rbp, rsp

	push rdx

	xor rdx,rdx

	mov dx, di
	in al, byte dx
	
	pop rdx

	mov rsp, rbp
	pop rbp
	ret

cleanActualRegisters:		; limpiamos todos los registros
	mov rax, 0
	mov rbx, 0
	mov rcx, 0
	mov rdx, 0
	mov rsi, 0
	mov rdi, 0
	mov r8, 0
	mov r9, 0
	mov r10, 0
	mov r11, 0
	mov r12, 0
	mov r13, 0
	mov r14, 0
	mov r15, 0
	ret

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