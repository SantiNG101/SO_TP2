#include <stdint.h>
#include <idt/exceptions.h>
#include <naiveConsole.h>

/*
 * Mantener actualizado. El EXCEPTION_TYPE_SIZE debe estar SIEMPRE al final.
 * Da, independientemente del contexto la cantidad de excepciones que hay.
 */
typedef enum { ZERO_DIVISION = 0, INVALID_OPERATION, DEFAULTEXCEPTION,EXCEPTION_TYPE_SIZE } EXCEPTION_T;

/* Arreglo de excepciones para que sea fácilmente mantenible. */
static void (* exceptions[EXCEPTION_TYPE_SIZE])(void) = { zeroDivisionException, invalidOperationException, defaultException };

void exceptionDispatcher(uint8_t ex){
    if(ex >= EXCEPTION_TYPE_SIZE)
        defaultException();
    exceptions[ex]();
}

void zeroDivisionException(){
    // TODO: COMPLETAR
    // Muestro los registros que hicieron error.
    showRegisterStatus();
    // Limpio los registros que dieron error
    cleanActualRegisters();
    // Imprimo el mensaje de error en rojo
    
}

void invalidOperationException(){
    // TODO: COMPLETAR
    // Tira excepción si el argumento no es valido
    // Imprimir en rojo que se introdujo una funcion no valida
    // llamar para que se imprima las funciones disponibles (esto puede ser en la funcion de bash)
}

void defaultException(){
    // TODO: COMPLETAR
    // Excepción por default. En caso de que algo male sal, entonces le decimos che flaco
    showRegisterStatus();       // muestro registros que dieron el error
    cleanActualRegisters();     // limpio los registros
                                // imprimo el mensaje de que salto la excepcion en rojo
}