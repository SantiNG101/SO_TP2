//
// syscall.c
//
#include <naiveConsole.h>
#include <keyboard.h>
#include <stdint.h>

/*
 *  Arguementos de una función de SYSCALL
 */
typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
} * argumentsStruct;

void write(argumentsStruct args);
void read(argumentsStruct args);

void (* syscalls[]) (argumentsStruct args) = { write, read };

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

void syscallDispatcher(argumentsStruct args){
    if(args->rdi > sizeofArr(syscalls)){
        ncPrint("Que so? boludo?, anda pa' alla bobo");
        ncNewline();
        ncPrint("La syscall: ");
        ncPrintDec(args->rdi);
        ncPrint(", no existe principe");
        ncNewline();
        return;
    }
    // Ejecuta la syscall
    syscalls[args->rdi]((argumentsStruct) args);
}

void read(argumentsStruct args){
    // This might read from keyboard buffer, print it and return it into the r10 Register.
    char*   data = (char *) args->r10;
    uint64_t len = (uint64_t) args->r9;
    
    getString(data, len);

    ncNewline();
}

void write(argumentsStruct args){
    char * c = (char *) args->r10;

    for(int i = 0; i < args->r9; i++)
        putChar(*c++);
}
