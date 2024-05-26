#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

#define BLOCKBYSYNC 5

#define MUTEX 0
#define WAITING 1

void initialize_scheduler();
void* schedule( char* actual_pointer);

#endif