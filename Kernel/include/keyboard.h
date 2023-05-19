//
// keyboard_Handler.h
//
#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H
#include <naiveConsole.h>

/*
 *  MACROS
 */


//Read port to get the scanned key
extern unsigned char read_port(unsigned char mode);

//getCharFunction
int getChar(void);

//scanf function
int scanf(const char * format, ...);

//getStringFunction [looped getChar until enter is pressed]
char * getString(void);

//getC Function
int getC(uint64_t status_port, uint64_t data_port);

void keyboardHandler();

#endif //KERNEL_KEYBOARD_H
