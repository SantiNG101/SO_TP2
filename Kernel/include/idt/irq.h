#ifndef _IRQ_H_
#define _IRQ_H_

#include <idt/loader.h>

/*
 * La interrupción propiamente armada. Lo que hace el dispatcher es procesar la petición.
 * Y por consiguiente llamar a la propia interrupción.
 */
void irqDispatcher(uint8_t irqID);

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _exception0Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

#endif