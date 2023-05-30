#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdint.h>
#include <strings.h>
#include <stdarg.h>

/*
 * TODO: Documentar correctamente.
 */

/*
 *  MACROS
 */
#define isdigit(c) (c >= '0' && c <= '9')
#define isspace(c) (c == ' ')

// calls to syscall Read.
extern char getChar(void);

// Uses getChar several times, until Carriage Return (CR) is pressed.
char * getString(void);

//scanf function
int scanf(const char * format, ...);

#endif