#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H
#include <stdint.h>
#include <stdarg.h>

//Screen painting functions
int puts(const char * string);
int putChar(int c);
int printf(const char * format, ...);

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
//

//KEYBOARD_HANDLERS
void handleBackSpace();
void handleTab();
void handleEnter();
void handleRArrow();
void handleLArrow();
void handleUPArrow();
void handleDOWNArrow();
// [Called in keyboard_Handler]

//Video driver
void writeMsg( char * str);
//

//Show register stats
void showRegisterStatus();
#endif