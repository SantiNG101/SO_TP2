#include <stdint.h>
#include <time.h>
#include <idt/irq.h>

/* LISTA DE INTERRUPCIONES */
typedef enum { TICK_IRQ = 0, IRQ_SIZE } IRQ_T;

// void (* irq)(void)[IRQ_SIZE] = { tick };

/* SI TODAVÍA NO EXISTE LA INTERRUPCIÓN NO HAGO NADA. */
void irqDispatcher(uint8_t irqID){
    if(irqID >= IRQ_SIZE)
        return;

    switch(irqID){
        case 0: 
            tick();
            break;

    }
    // irq[irqID]();
}