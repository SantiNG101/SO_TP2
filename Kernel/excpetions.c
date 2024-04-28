//
// File: exceptions.c
// Basado en la lista de excepciones de: https://wiki.osdev.org/Exceptions
//

#include <idt/exceptions.h>
#include <keyboard.h>
void cleanActualRegisters();

/* Arreglo de excepciones para que sea fácilmente mantenible. */
static void (* exceptions[])(uint64_t * rip) = { zeroDivisionException, invalidOperationException };

uint64_t exceptionDispatcher(uint8_t ex, uint64_t rip, argumentsStruct args, uint64_t oldRSP){
    if(ex >= sizeof(exceptions) / sizeof(exceptions[0]))
        return -1;

    uint32_t lForeground = getForegroundColour();
    setForegroundColour(RED);
    print("-------- [ KERNEL EXCEPTION ] - Exception ");
    printDec(ex);
    print(" @ 0x");
    // This works.
    printHex(rip);
    print(" --------");
    enter();

    setForegroundColour(lForeground);
    // Imprimo los registros.
    showRegisterStatus(args, oldRSP);
    enter();
    setForegroundColour(RED);

    // Controlo la excepción
    exceptions[ex](&rip);
    enter();

    setForegroundColour(lForeground);
    print("Press any key to continue []:");
    while((getC())==0);
    return rip;
}

void zeroDivisionException(uint64_t * rip){
    printErrorStr("Exception: Division by zero");
    enter();
    

    // Debo hacer que continué la ejecución. Porque es un Fault Exception.
    // Primero debo modificar los registros, que causaron el problema.
    cleanActualRegisters();
    
    *rip = 0x400000;
    return;
}

void invalidOperationException(uint64_t * rip){
    printErrorStr("Exception: Invalid OpCode");
    enter();

    cleanActualRegisters();     
    *rip = 0x400000;
    return;
}

void printRegister(char * str, uint64_t regValue){
    print(str);
    printHex(regValue);
    enter();
}