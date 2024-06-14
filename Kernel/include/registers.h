#ifndef _REGISTERS_H_
#define _REGISTERS_H_
#include <screen.h>

extern uint64_t getCurrentRAX();
extern uint64_t getCurrentRBX();
extern uint64_t getCurrentRCX();
extern uint64_t getCurrentRDX();
extern uint64_t getCurrentRBP();
extern uint64_t getCurrentRSP();
extern uint64_t getCurrentRDI();
extern uint64_t getCurrentRSI();
extern uint64_t getCurrentR15();
extern uint64_t getCurrentR14();
extern uint64_t getCurrentR13();
extern uint64_t getCurrentR12();
extern uint64_t getCurrentR11();
extern uint64_t getCurrentR10();
extern uint64_t getCurrentR9();
extern uint64_t getCurrentR8();


typedef struct RegArgs{
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
    uint64_t prevRBP;
} * argumentsStruct;

/**
    funcion que guarda los registros y luego los imprime en pantalla
    especialmente para excepciones
**/
void showRegisterStatus(argumentsStruct args, uint64_t oldRSP);

// guarda los registros en un array con capacidad para todos los registros (F11)
void saveRegisterStatus(argumentsStruct args, uint64_t oldRSP);

// imprime los registros guardados en pantalla en el momentos que se quiera
void printRegisterStatus();
void printRegister(char* str, uint64_t regs);
uint64_t* getRegisters();
#endif
