#include <registers.h>
//ILLEGAL
static uint64_t regs[16] = {0};

void showRegisterStatus(){
    enter();
    saveRegisterStatus();
    printRegisterStatus();
    enter();
}

void saveRegisterStatus(){
    regs[0] = getCurrentRAX();
    regs[1] = getCurrentRBX();
    regs[2] = getCurrentRCX();
    regs[3] = getCurrentRDX();
    regs[4] = getCurrentRBP();
    regs[5] = getCurrentRSI();
    regs[6] = getCurrentRDI();
    regs[7] = getCurrentR15();
    regs[8] = getCurrentR14();
    regs[9] = getCurrentR13();
    regs[10] = getCurrentR12();
    regs[11] = getCurrentR11();
    regs[12] = getCurrentR10();
    regs[13] = getCurrentR9();
    regs[14] = getCurrentR8();
    regs[15] = getCurrentRSP();
}

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