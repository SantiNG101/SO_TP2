#include <stdint.h>
#include <defs.h>
#include <idt/irq.h>

#pragma pack(push)      // Push de alineación actual
#pragma pack(1)         // Alineamiento por 1 Byte.
/***
--- GATE DESCRIPTOR ---
***/
typedef struct {
   uint16_t offset_l, selector;
   uint8_t  ist, attributes;            
   uint16_t offset_m;
   uint32_t offset_h, zero;
} DESCR_INT;

#pragma pack(pop)

/* Armo el IDT propiamente dicho. Son 255 entradas. */
DESCR_INT * idt = (DESCR_INT *) 0;

static void setup_IDT_entry(uint8_t, uint64_t, IRQ_T);

/* */
void load_idt(){
    // Comienzo a agregar los IDTR
    /* INTERRUPCIONES */
    setup_IDT_entry(0x20, &_irq00Handler, HARDWAR_I);           // Seteo el timerTick en el 20h
    setup_IDT_entry(0x21, &_irq01Handler, HARDWAR_I);           // Seteo el keyboardHandler en el 21h

    setup_IDT_entry(0x80, &_irq02Handler, SYSCALL_I);           // Seteo la syscall.

    /* Cargo las excepciones */
    setup_IDT_entry(0x00, &_exception00Handler, EXCEPTI_I);     // Error al dividir
    setup_IDT_entry(0x06, &_exception01Handler, EXCEPTI_I);     // Operación invalida.

    picMasterMask(0xFC);
    picSlaveMask(0xFF);

    _sti();
}

static void setup_IDT_entry(uint8_t index, uint64_t offset, IRQ_T type){
    idt[index].selector = 0x08;
    idt[index].offset_l = offset & 0xFFFF;               
    idt[index].offset_m = (offset >> 16) & 0xFFFF;      
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;  
    /*
     * Pongo uno de los tipos de interrupción. Puede ser
     * de hardware, o por el usuario. También puede ser una excepción.
     * (HARDWAR_I, SYSCALL_I, EXCEPTI_I)
     */
    idt[index].attributes = type;                       
    idt[index].ist = 0;
    idt[index].zero = 0;
}

