#include <semaphore.h>
#include <semaphores.h>
#include <mm.h>
#include <lib.h>
#include <stdlib.h>
#include "include/idt/irq.h"
#include "include/sync.h"
#include <scheduler.h>
#include <process.h>

// Function to create a semaphore
semaphore_ptr create_semaphore(char *name, int value) {
    semaphore_ptr sem = memalloc(sizeof(semaphore_t));
    memcpy(sem->name, name, MAX_NAME_LENGTH - 1);
    sem->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    sem->value = value;
    sem->lock = 0;
    sem->blocked_qty = 0;
    return sem;
}

// Function to wait (decrement) on a semaphore
void semaphore_wait(semaphore_ptr sem) {
    enterRegion(&sem->lock);
    int pid = get_pid();
    while (1) { //Sugerencia: El enterRegion ya haría la parte del loop
        if (sem->value == 0) {
            sem->blocked_processes[sem->blocked_qty++]=pid;
            leaveRegion(&sem->lock, MUTEX);
            set_status(pid, BLOCKED);
            yield();
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
    unblock_all_p(sem);
    if(sem->value==0) {
        sem->value++;
    }
    leaveRegion(&sem->lock, MUTEX);
}

void unblock_all_p(semaphore_ptr sem){
    for (int i=0; i<sem->blocked_qty; i++){
        set_status(sem->blocked_processes[i], READY);
        yield();
    }
    sem->blocked_qty = 0;
}

// Function to destroy a semaphore
void destroy_semaphore(semaphore_ptr sem) {
    free(sem);
}

// Function to get the name of a semaphore
char *get_semaphore_name(semaphore_ptr sem) {
    return sem->name;
}
