#include <stdint.h>
#include <time.h>
#include <keyboard.h>
#include <idt/irq.h>
#include <registers.h>

#include <syscall.h>

/* LISTA DE INTERRUPCIONES */
typedef enum { TICK_IRQ = 0, KEYBOARD_IRQ, IRQ_SIZE } IRQ_T;

void (* irq [IRQ_SIZE])(void) = { tick };

/* SI TODAVÍA NO EXISTE LA INTERRUPCIÓN NO HAGO NADA. */
void irqDispatcher(uint8_t irqID, argumentsStruct args, uint64_t oldRSP){
    if(irqID >= IRQ_SIZE)
        return;

    if(irqID == KEYBOARD_IRQ) return keyboardHandler(args, oldRSP);
    if(irqID == TICK_IRQ) return from_tick(args);
    irq[irqID]();
}