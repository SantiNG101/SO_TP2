//
// Created by ptorl on 5/17/2023.
//

#ifndef TPEARQUI_TERMINALHANDLER_H
#define TPEARQUI_TERMINALHANDLER_H

/*
 * Librerías requeridas para el correcto funcionamiento de la terminal.
 */
#include <stdint.h>
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#include "songs.h"
#include "figures.h"
#include "pong.h"
#include "snake.h"
#include "libc/include/stdio.h"
#include "tests.h"
#include "libc/include/stdlib.h"

#define TRUE 1
#define FALSE 0
//HashCodes (obtained from hashing in range)
#define H_HELP 35
#define H_EXIT 28
#define H_TIME 5
#define H_CLEAR 7
//

//Las unicas 2 funciones visibles

//Arranca el bash
int shell(int argc, char* argv[]);
int64_t hashInRange(const char* str, uint64_t start, uint64_t end);
/*
 * Formato de comandos aceptados por la terminal:
 *
 * 0        1       2       3       4       ...  N
 * COMANDO   ARG0   ARG1    ARG2    ARG3    ... ARGN-1
 */
#endif //TPEARQUI_TERMINALHANDLER_H
