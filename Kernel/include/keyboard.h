//
// keyboard_Handler.h
//
#ifndef KERNEL_KEYBOARD_HANDLER_H
#define KERNEL_KEYBOARD_HANDLER_H
#include <stdint.h>
#include <naiveConsole.h>
#include <registers.h>
#include <mm.h>
#include <scheduler.h>

#define toMinus(c) (c | 0x20)
#define isalpha(c) (toMinus(c) >= 'a' && toMinus(c) <= 'z')
#define toMayus(c) (isalpha(c) ? c & 0x5F : c)
 
//Read port to get the scanned key
extern unsigned char read_port(unsigned char mode);

//getC Function
int getC();

void keyboardHandler(argumentsStruct args, uint64_t oldRSP);
int getKeyState(int keyCode);

int buff_is_full();

#endif KERNEL_KEYBOARD_HANDLER_H
