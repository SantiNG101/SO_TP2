#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm.h>
#include <registers.h>

#define NULL 0

// Process states
#define BLOCKED 0
#define READY 1
#define RUNNING 2
#define EMBRYO 3
#define ZOMBIE 4

// Process Priority

#define MOSTIMP 10
#define MEDIUMIMP 5
#define LESSIMP 1

#define STACK_MEM 0x1000       // 4K of mem
#define PROCESS_MEM 0x100000  // 1M of mem

typedef struct pcb* pcb_pointer;

void process_init();
void process_create( int pidParent, uint64_t rip, int argc, char* argv[] );
uint64_t* align_stack( uint64_t* init);

#endif