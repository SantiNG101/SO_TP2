#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm.h>
#include <registers.h>
#include <scheduler.h>
#include <naiveConsole.h>

#define NULL 0

// Process states
#define BLOCKED 0
#define READY 1
#define RUNNING 2
#define EMBRYO 3
#define ZOMBIE 4

// Process Priority

#define MOSTIMP 0
#define MEDIUMIMP 1
#define LESSIMP 2
#define ALWAYSACTIVE 3

#define STACK_MEM 0x1000       // 4K of mem
#define PROCESS_MEM 0x100000  // 1M of mem

struct sch_info{
    int p_state;
    int priority;
    uint64_t* pqueue;                //pointer to scheduling queue
    int CPU_time;
                                    // posible other characteristics that are going to be used by the scheduler to determine its priority
};


typedef struct pcb* pcb_pointer;

void process_init();
int process_create( int pidParent, uint64_t rip, int argc, char* argv[] );
uint64_t* align_stack( uint64_t* init);

#endif