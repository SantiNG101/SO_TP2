//
// Created by ptorl on 5/17/2023.
//

#ifndef TPEARQUI_TERMINALHANDLER_H
#define TPEARQUI_TERMINALHANDLER_H

#define TRUE 1
#define FALSE 0
//HashCodes (obtained from hashing in range)
#define H_HELP 35
#define H_EXIT 28
#define H_TIME 5
#define H_CLEAR 7
//
#include <stdint.h>
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
