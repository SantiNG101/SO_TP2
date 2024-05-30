//
// Created by Santi on 5/29/2024.
//

#ifndef SO_TP2_SEMAPHORES_H
#define SO_TP2_SEMAPHORES_H

#define MAX_NAME_LENGTH 40
#define MAX_PROCESS_AMOUNT 10
#define MAX_NAME_LENGTH 40
#define MAX_PROCESS_AMOUNT 10
#define MAX_SEMAPHORES 100

typedef struct semaphore {
    int lock;
    int value;
    char name[MAX_NAME_LENGTH];
    int blockedProcesses[MAX_PROCESS_AMOUNT];
    int amountBlocked;
    //arreglo de procesos  bloqueados por el semáforo
} semaphore_t, *semaphore_ptr;

semaphore_ptr create_semaphore(char *name, int value);

// Function to wait (decrement) on a semaphore
void semaphore_wait(semaphore_ptr sem);

// Function to post (increment) on a semaphore
void semaphore_post(semaphore_ptr sem);

// Function to destroy a semaphore
void destroy_semaphore(semaphore_ptr sem);

// Function to get the name of a semaphore
char *get_semaphore_name(semaphore_ptr sem);
#endif //SO_TP2_SEMAPHORES_H
