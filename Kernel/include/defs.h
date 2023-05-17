/* ++------------------------------------++ */
/*                defs.h                    */
/* ++------------------------------------++ */

// Descripción del archivo:
// TODO: Agregar descricipción.

#ifndef _DEFS_H_
#define _DEFS_H_

    /* Definiciones para las IDT */
    #define ACCS_P      0x80                                   // El bit de presencia siempre activo.
    #define ACCS_INT    0x0E                                   // Interrupción
    #define ACCS_EXC    0x8F                                   // Excepción
    #define ACCS_USR    0x60                                   // Accedible por el usuario. (0x00 Kernel)

    typedef enum{ 
                    SYSCALL_I = ( ACCS_P | ACCS_INT | ACCS_USR ),       // Para una syscall
                    HARDWAR_I = ( ACCS_P | ACCS_INT ),                   // Para una interrupción de hardware
                    EXCEPTI_I = ACCS_EXC
                } IRQ_T;                  
#endif


