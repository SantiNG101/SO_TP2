#include <stdint.h>
#include <idt/exceptions.h>
#include <naiveConsole.h>

/*
 * Mantener actualizado. El EXCEPTION_TYPE_SIZE debe estar SIEMPRE al final.
 * Da, independientemente del contexto la cantidad de excepciones que hay.
 */
typedef enum { ZERO_DIVISION = 0, INVALID_OPERATION, EXCEPTION_TYPE_SIZE } EXCEPTION_T;

/* Arreglo de excepciones para que sea fácilmente mantenible. */
static void (* exceptions[EXCEPTION_TYPE_SIZE])(void) = { zeroDivisionException, invalidOperationException };

void exceptionDispatcher(uint8_t ex){
    if(ex >= EXCEPTION_TYPE_SIZE)
        notKnownException();

    exceptions[ex]();
}

void zeroDivisionException(){
    // TODO: COMPLETAR
    // Muestro los registros que hicieron error.
    showRegisterStatus();
    // Limpio los registros que dieron error
    cleanRegisterStatus();
    // Imprimo el mensaje de error
    
}

void invalidOperationException(){
    // TODO: COMPLETAR
    // Tira excepción si el argumento no es valido

}

void notKnownException(){
    // TODO: COMPLETAR

    // Excepción por default. En caso de que algo male sal, entonces le decimos che flaco
    // Esta excepción no existe, pero aunque sea te tiro algo igual..
    // Soy re buena onda ;)
}