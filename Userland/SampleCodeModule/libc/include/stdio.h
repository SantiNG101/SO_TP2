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
extern void putPixel(uint32_t x, uint32_t y, uint32_t hexColor);
extern void updateScreen(void);
extern void setBuffer(int mode);
extern int getKeyState(int keyCode);
extern void clearScreen(int mode);
extern void setFontSize(int size);
extern void setPrintAnywhere(uint32_t y, uint32_t x);

//
// functionName: setBackgroundColour
// Description: Cambia el color del fondo.
// params:
//      * (uint32_t) colour - Color del fondo a imprimir
//
extern void setBackgroundColour(uint32_t colour);
//
// functionName: setForegroundColour
// Description: Cambia el color del texto.
// params:
//      * (uint32_t) colour - Color del texto a imprimir
//
extern void setForegroundColour(uint32_t colour);

// Uses getChar several times, until Carriage Return (CR) is pressed.
char * getString(void);

//scanf function
int scanf(const char * fmt, ...);

//Screen painting functions
int puts(const char * str);
int printf(const char * fmt, ...);



#endif