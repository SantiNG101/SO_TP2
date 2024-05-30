#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef struct list* p_list;

void initialize_scheduler();

void add_process_to_scheduling( int pid, struct sch_info * process_info, uint8_t* rsp);

void delete_process_scheduling( int pid );

uint8_t* schedule( uint8_t* actual_pointer);

p_list getReadyToRun();

int get_pid();

int blocked_to_scheduling( int _pid);

int scheduling_to_blocked();

#endif