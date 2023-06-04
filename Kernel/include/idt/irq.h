#ifndef _IRQ_H_
#define _IRQ_H_

#include <idt/loader.h>

/*
 * La interrupción propiamente armada. Lo que hace el dispatcher es procesar la petición.
 * Y por consiguiente llamar a la propia interrupción.
 */
void irqDispatcher(uint8_t irqID);

void _irq00Handler(void);   // timer tick
void _irq01Handler(void);   // Keyboard
void _irq02Handler(void);   // User

void _exception00Handler(void);  // Division by zero exception
void _exception01Handler(void);  // Not an available function


void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

#endif