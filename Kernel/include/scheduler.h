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
int add_to_priority_list( p_list process );
p_list find_with_remove( int _pid );
p_list remove_in_scheduling_by_level( int _pid, int lvl );
p_list getReadyToRun();
p_list search_from_top();
p_list look_in_list ( int lvl , p_list from );

#endif