
/*
 * Mantener actualizado. El EXCEPTION_TYPE_SIZE debe estar SIEMPRE al final.
 */
typedef enum { ZERO_DIVISION = 0, INVALID_OPERATION, EXCEPTION_TYPE_SIZE } EXCEPTION_T;


/* ----------- EXCEPTIONS ----------- */

void zeroDivisionException();
void invalidOperationException();
void notKnownException();

/* --------------------------------- */

/* Arreglo de excepciones para que sea fácilmente mantenible. */
(void) exceptions[EXCEPTION_TYPE_SIZE] = { zeroDivisionException, invalidOperationException };

void exceptionDispatcher(EXCEPTION_T ex){
    if(ex = EXCEPTION_TYPE_SIZE)
        notKnownException();

    exceptions[ex]();
}

void zeroDivisionException(){
    // Debo fijarme que el registro de Division por 0 no esté activado.

}

void invalidOperationException(){
    // Tira excepción si el argumento no es valido
}

void notKnownException(){
    // Excepción por default. En caso de que algo male sal, entonces le decimos che flaco
    // Esta excepción no existe flaco, pero aunque sea te tiro la excepción viste?
    // Soy re buena onda ;)
}