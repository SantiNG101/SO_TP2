//
// keyboard_Handler.h
//
#ifndef KERNEL_KEYBOARD_HANDLER_H
#define KERNEL_KEYBOARD_HANDLER_H
#include <stdint.h>
#include <naiveConsole.h>
#include <registers.h>
#define toMinus(c) (c | 0x20)
#define isalpha(c) (toMinus(c) >= 'a' && toMinus(c) <= 'z')
#define toMayus(c) (isalpha(c) ? c & 0x5F : c)

/*
 * TODO: Documentar correctamente.
 */
 
//Read port to get the scanned key
extern unsigned char read_port(unsigned char mode);

//getC Function
int getC();

void keyboardHandler(argumentsStruct args, uint64_t oldRSP);

#endif //KERNEL_KEYBOARD_HANDLER_H
