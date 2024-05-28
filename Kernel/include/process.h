#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm.h>
#include <registers.h>
#include <scheduler.h>
#include <naiveConsole.h>

#define NULL ((void *)0)

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

#define STACK_MEM 4096       // 4K of mem
#define PROCESS_MEM 0x100000  // 1M of mem

struct sch_info{
    int p_state;
    int priority;
    uint8_t* pqueue;                //pointer to scheduling queue
    int CPU_time;
                                    // posible other characteristics that are going to be used by the scheduler to determine its priority
};


typedef struct pcb* pcb_pointer;

extern uint8_t* prepare_process(uint8_t* stack, uint8_t* rip, int argc, char* argv[]);

void process_init();
int process_create( int pidParent, uint8_t* rip, int argc, char* argv[] );
uint8_t* align_stack( uint8_t* init);

#endif