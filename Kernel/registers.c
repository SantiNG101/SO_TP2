#include <registers.h>
//ILLEGAL
static uint64_t regs[16] = {0};

void showRegisterStatus(argumentsStruct args, uint64_t oldRSP){
    enter();
    saveRegisterStatus(args, oldRSP);
    printRegisterStatus();
    enter();
}

void saveRegisterStatus(argumentsStruct args, uint64_t oldRSP){
    regs[0] = args->rax;
    regs[1] = args->rbx;
    regs[2] = args->rcx;
    regs[3] = args->rdx;
    regs[4] = args->prevRBP;
    regs[5] = args->rsi;
    regs[15] = oldRSP;
    regs[6] = args->rdi;
    regs[7] = args->r15;
    regs[8] = args->r14;
    regs[9] = args->r13;
    regs[10] = args->r12;
    regs[11] = args->r11;
    regs[12] = args->r10;
    regs[13] = args->r9;
    regs[14] = args->r8;
}

uint64_t* getRegisters(){
    regs[0] = getCurrentRAX();
    regs[1] = getCurrentRBX();
    regs[2] = getCurrentRCX();
    regs[3] = getCurrentRDX();
    regs[4] = getCurrentRBP();
    regs[5] = getCurrentRDI();
    regs[6] = getCurrentRSI();
    regs[7] = getCurrentR8();
    regs[8] = getCurrentR9();
    regs[9] = getCurrentR10();
    regs[10] = getCurrentR11();
    regs[11] = getCurrentR12();
    regs[12] = getCurrentR13();
    regs[13] = getCurrentR14();
    regs[14] = getCurrentR15();
    regs[15] = getCurrentRSP();

    return regs;
}

// void printRegister(char * str, uint64_t regValue){
//     print(str);
//     printHex(regValue);
//     enter();
// }

void printRegisterStatus(){
    printRegister("A: ", regs[0]);
    printRegister("B: ", regs[1]);
    printRegister("C: ", regs[2]);
    printRegister("D: ", regs[3]);
    printRegister("BP: ", regs[4]);
    printRegister("SI: ", regs[5]);
    printRegister("SP: ", regs[15]);
    printRegister("DI: ", regs[6]);
    printRegister("15: ", regs[7]);
    printRegister("14: ", regs[8]);
    printRegister("13: ", regs[9]);
    printRegister("12: ", regs[10]);
    printRegister("11: ", regs[11]);
    printRegister("10: ", regs[12]);
    printRegister("9: ", regs[13]);
    printRegister("8: ", regs[14]);
}