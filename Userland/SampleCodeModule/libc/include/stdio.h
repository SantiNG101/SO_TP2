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

// calls to syscall Read/write.
extern char getChar(void);
extern char putChar(char c);

// Uses getChar several times, until Carriage Return (CR) is pressed.
char * getString(void);

//scanf function
int scanf(const char * fmt, ...);

//Screen painting functions
int puts(const char * str);
int printf(const char * fmt, ...);

#endif