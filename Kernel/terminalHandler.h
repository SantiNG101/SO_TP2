#ifndef TPEARQUI_TERMINALHANDLER_H
#define TPEARQUI_TERMINALHANDLER_H

#define IN_LINE_BUFFER 79   //descuenta el $ indicando que espera que se pase un comando
#define MAX_LINE_ARGS 6
#define TRUE 1
#define FALSE 0

//Las unicas 2 funciones visibles

//Arranca el bash
int terminalStart();

/*
 * Formato de comandos aceptados por la terminal:
 *
 * 0        1       2       3       4       ...  N
 * COMANDO   ARG0   ARG1    ARG2    ARG3    ... ARGN-1
 */
#endif //TPEARQUI_TERMINALHANDLER_H
