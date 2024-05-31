#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm.h>
#include <registers.h>
#include <scheduler.h>
#include <naiveConsole.h>
#include <screen.h>

#define NULL ((void *)0)
#define SUCCESS 0
#define ERROR 1
#define NOTFOUND -1

// Process states
#define BLOCKED 0
#define READY 1
#define RUNNING 2
#define EMBRYO 3
#define ZOMBIE 4

// Process Fd
#define STDIN 0
#define STDOUT 1
#define STDERR 2

// Process Priority

#define MOSTIMP 0
#define MEDIUMIMP 1
#define LESSIMP 2
#define ALWAYSACTIVE 3

#define STACK_MEM 4096       // 4K of mem
#define PROCESS_MEM 0x100000  // 1M of mem
#define MAX_PROCESSES_SUPPORTED 1000

struct sch_info{
    int p_state;
    int priority;
    uint8_t* pqueue;                //pointer to scheduling queue
    int CPU_time;
                                    // posible other characteristics that are going to be used by the scheduler to determine its priority
};


typedef struct pcb* pcb_pointer;

extern uint8_t* prepare_process(uint8_t* stack, uint8_t* rip, int argc, char* argv[]);
extern void forceTimerTick();
extern void _cli(void);

extern void _sti(void);

extern void _hlt(void);

void process_init();
int process_create( int pidParent, uint8_t* rip, int argc, char* argv[], int foreground );
uint8_t* align_stack( uint8_t* init);
void show_processes();
void change_rsp_process( int pid, uint8_t* rsp );
int get_pid_parent();

#endif