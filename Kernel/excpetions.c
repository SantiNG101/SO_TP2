//
// File: exceptions.c
// Basado en la lista de excepciones de: https://wiki.osdev.org/Exceptions
//
#include <stdint.h>
#include <idt/exceptions.h>
#include <naiveConsole.h>

/*
 * Mantener actualizado. El EXCEPTION_TYPE_SIZE debe estar SIEMPRE al final.
 * Da, independientemente del contexto la cantidad de excepciones que hay.
 */
// typedef enum { ZERO_DIVISION = 0, INVALID_OPERATION, DEFAULTEXCEPTION,EXCEPTION_TYPE_SIZE } EXCEPTION_T;

/* Arreglo de excepciones para que sea fácilmente mantenible. */
static void (* exceptions[])(uint64_t * rip) = { zeroDivisionException, invalidOperationException, defaultException };

uint64_t exceptionDispatcher(uint8_t ex, uint64_t rip){
    if(ex >= sizeof(exceptions) / sizeof(exceptions[0]))
        defaultException(&rip);

    // A partir de ahora se pone el texto siempre en rojito.
    // ncColor(0xFF0000);
    ncPrint("Kernel Exception - Exception ");
    ncPrintDec(ex);
    ncPrint(" @ 0x");
    ncPrintHex(rip);
    ncNewline();

    // Imprimo los registros.
    showRegisterStatus();
    ncNewline();
    // Llamo la razón por la que hubo excepción.
    
    exceptions[ex](&rip);

    // Cambio el texto otra vez a la normalite.
    // ncColor(normal);

    return rip;
}

void zeroDivisionException(uint64_t * rip){
    ncPrint("Exception: Division by zero");
    ncNewline();

    // Debo hacer que continué la ejecución. Porque es un Fault Exception.
    // Primero debo modificar los registros, que causaron el problema.
    cleanActualRegisters();
    // Luego, debo cambiar el RIP para que ejecute la siguiente linea.
    *rip += 4;
    return;
}

void invalidOperationException(uint64_t * rip){
    // TODO: COMPLETAR
    ncPrint("Exception: Invalid OpCode");
    ncNewline();

    cleanActualRegisters();     
    *rip += 1;                  // Paso a la siguiente instrucción.
    return;
}

void defaultException(uint64_t * rip){
    // // TODO: COMPLETAR
    // // Excepción por default. En caso de que algo male sal, entonces le decimos che flaco
    // showRegisterStatus();       // muestro registros que dieron el error
    // cleanActualRegisters();     // limpio los registros
    //                             // imprimo el mensaje de que salto la excepcion en rojo
}