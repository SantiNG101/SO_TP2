#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef struct list* p_list;

void initialize_scheduler();

void addProcessToScheduling( int pid, struct sch_info * process_info, uint64_t rip);

void deleteProcessScheduling( int pid );


uint64_t* schedule( uint64_t* actual_pointer);


// A blocking function call this function to change the process for it to wait
void blocking( uint64_t* actual_pointer );

// function called by timer tick to get rid of a running process
void from_tick( uint64_t* actual_pointer );

p_list getRunning();

p_list getReadyToRun();

#endif