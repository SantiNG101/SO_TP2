#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef struct list* p_list;

void initialize_scheduler();

void add_process_to_scheduling( int pid, struct sch_info * process_info, uint8_t* rsp);

void deleteProcessScheduling( int pid );


uint8_t* schedule( uint8_t* actual_pointer);


// A blocking function call this function to change the process for it to wait
void blocking( uint8_t* actual_pointer );

// function called by timer tick to get rid of a running process
uint8_t* from_tick( uint8_t* actual_pointer );

p_list getRunning();

p_list getReadyToRun();

#endif