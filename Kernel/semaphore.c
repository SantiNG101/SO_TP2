#include <semaphore.h>
#include <semaphores.h>
#include <mm.h>
#include <lib.h>
#include <stdlib.h>
#include "include/idt/irq.h"
#include "include/sync.h"
#include <scheduler.h>

// Function to create a semaphore
semaphore_ptr create_semaphore(char *name, int value) {
    semaphore_ptr sem = myMalloc(sizeof(semaphore_t));
    memcpy(sem->name, name, MAX_NAME_LENGTH - 1);
    sem->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    sem->value = value;
    sem->lock = 0;
    return sem;
}

// Function to wait (decrement) on a semaphore
void semaphore_wait(semaphore_ptr sem) {
    enterRegion(&sem->lock);
    while (1) { //Sugerencia: El enterRegion ya haría la parte del loop
        if (sem->value == 0) {
            sem->blockedProcesses[sem->amountBlocked++];//TODO = getcurrentpid;
            leaveRegion(&sem->lock, MUTEX);
            //blockRunningProcess function
            enterRegion(&sem->lock);
        } else {
            break;
        }
    }
    sem->value--;
    leaveRegion(&sem->lock, MUTEX);
}

// Function to post (increment) on a semaphore
void semaphore_post(semaphore_ptr sem) {
    enterRegion(&sem->lock);
    if(sem->value==0) {
        sem->value++;
    }
    leaveRegion(&sem->lock, MUTEX);
    //tryToUnlockSemaphore function;
}

// Function to destroy a semaphore
void destroy_semaphore(semaphore_ptr sem) {
    myFree(sem);
}

// Function to get the name of a semaphore
char *get_semaphore_name(semaphore_ptr sem) {
    return sem->name;
}