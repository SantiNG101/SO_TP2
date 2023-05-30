#include <stdint.h>
#include <time.h>
#include <keyboard.h>
#include <idt/irq.h>

#include <syscall.h>

/* LISTA DE INTERRUPCIONES */
typedef enum { TICK_IRQ = 0, KEYBOARD_IRQ, IRQ_SIZE } IRQ_T;

void (* irq [IRQ_SIZE])(void) = { tick, keyboardHandler };

/* SI TODAVÍA NO EXISTE LA INTERRUPCIÓN NO HAGO NADA. */
void irqDispatcher(uint8_t irqID){
    if(irqID >= IRQ_SIZE)
        return;

    irq[irqID]();
}