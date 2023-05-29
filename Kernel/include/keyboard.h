//
// keyboard_Handler.h
//
#ifndef KERNEL_KEYBOARD_HANDLER_H
#define KERNEL_KEYBOARD_HANDLER_H
#include <naiveConsole.h>

/*
 *  MACROS
 */
#define isdigit(c) (c >= '0' && c <= '9')
#define isspace(c) (c == ' ')

//Read port to get the scanned key
extern unsigned char read_port(unsigned char mode);

// //getCharFunction
// int getChar(void);

//scanf function
int scanf(const char * format, ...);

//getStringFunction [looped getChar until enter is pressed]
char * getString(char * data, uint64_t len);

//getC Function
int getChar();

int getC();

void keyboardHandler();

#endif //KERNEL_KEYBOARD_HANDLER_H
