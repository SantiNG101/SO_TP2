#ifndef ___EXCEPTIONS
#define ___EXCEPTIONS

#include <stdint.h>
#include <registers.h>
#include <screen.h>

// Como máximo da lugar a 256 excepciones, en caso de que se ingrese un número desde
// ASM MAYOR a 8bits, C solo va considerar los primeros 8.
uint64_t exceptionDispatcher(uint8_t ex, uint64_t rip, argumentsStruct args, uint64_t oldRSP);

/*
 * Excepcion 0
 */
void zeroDivisionException(uint64_t * rip); 

/*
 * Excepcion 1
 */
void invalidOperationException(uint64_t * rip);


#endif