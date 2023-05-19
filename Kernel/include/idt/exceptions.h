#ifndef ___EXCEPTIONS
#define ___EXCEPTIONS

// Como máximo da lugar a 256 excepciones, en caso de que se ingrese un número desde
// ASM MAYOR a 8bits, C solo va considerar los primeros 8.
void exceptionDispatcher(uint8_t ex);

/*
 * Excepcion 0
 */
void zeroDivisionException(); 

/*
 * Excepcion 1
 */
void invalidOperationException();

/*
 * Excepcion default, para cuando aún no se ha programado la excepcion.
 * Por ejemplo: En este caso, la excepcion 230 no existe, por lo que dará una notKnownException.
 */
void defaultException();

#endif