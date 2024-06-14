#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"


typedef struct list* p_list;

#define BLOCKBYSYNC 5
#define MUTEX 0
#define WAITING 1


void initialize_scheduler();

void add_process_to_scheduling( int pid, struct sch_info * process_info, uint8_t* rsp);

int delete_process_scheduling( int _pid );

uint8_t* schedule( uint8_t* actual_pointer);

p_list getReadyToRun();

int get_pid();

int blocked_to_scheduling( int _pid);

int scheduling_to_blocked(int _pid);

int change_priority( int _pid, int new_priority );
void yield();
void arrange_priorities();

#endif