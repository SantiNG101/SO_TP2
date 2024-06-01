//
// syscall.c
//
#include <naiveConsole.h>
#include <keyboard.h>
#include <stdint.h>
#include <screen.h>
#include <speaker.h>
#include <time.h>
#include <videoDriver.h>
#include <registers.h>
#include <process.h>
#include <scheduler.h>
#include <semaphore.h>
#include <semaphores.h>
#include <lib.h>
#include <stdlib.h>
#include "include/idt/irq.h"
#include "include/sync.h"
#include <syscall.h>

#define MAX_SEMAPHORES 100

static semaphore_ptr semaphores[MAX_SEMAPHORES];
static int sem_count = 0;
/*
 *  Argumentos de una función de SYSCALL
 */

void read(argumentsStruct args){
    // This might read from keyboard buffer, print it and return it into the r10 Register.
    char*   data = (char *) args->r10;
    uint64_t len = (uint64_t) args->r9;
    char    c;

    while(len){
        while((c = getC()) == 0);
        *data++ = c;
        len--;
    }
}

void write(argumentsStruct args){
    char * c = (char *) args->r10;
    for(int i = 0; i < args->r9; i++){
        switch(*c){
            case '\n':
                enter();
                break;
            case '\a':
                beep();
                break;
            case '\b':
                backspace();
                break;
            case '\t':
                tab();
                break;
            case '\0':
                break;
            case 27:            //caso del esc retorna su numero ascii
                break;
            default:
                putCharScreen(*c);
                break;
        }

        c++;
    }
}

void setterBuffer(argumentsStruct args){
    setDoubleBuffer(args->r10);
}

void setFontSize(argumentsStruct args){
    setScale(args->r10);
}

void timer_wait(argumentsStruct args){
    wait(args->r10);
}

void speaker_playSound(argumentsStruct args){
    beepCustom(args->r10, args->r9);
}

void timeNow(argumentsStruct args){
    args->r10 = getTime();
}

void clean(argumentsStruct args){
    clearScreen(args->r10);
}

//r10 x r9 y r8 color
void putPix(argumentsStruct args){
    putPixel(args->r10,args->r9,args->r8);
}

void updtScreen(argumentsStruct args){
    updateScreen();
}

void foreGround(argumentsStruct args){
    setForegroundColour(args->r10);
}

void backGround(argumentsStruct args){
    setBackgroundColour(args->r10);
}

void keyState(argumentsStruct args){
    args->rsi = getKeyState(args->r10);
}

void showRegisters(argumentsStruct args){
    printRegisterStatus();
}

void setPrintAnywhere(argumentsStruct args){
    setPrintingMode(args->r10,args->r9,0);
}


// Starts a sound. With the given frq
void startS(argumentsStruct args){
    playSound(args->r10);
}

// Stops the sound.
void stopS(argumentsStruct args){
    stopSound();
}

// rdi, rsi, rdx, rcx, r8 and r9 in that order
void execve(argumentsStruct args){
    process_create(args->rsi, args->rdx, args->rcx, args->r8,1);
}

int syscall_create_semaphore(char *name, int value) {
    if (sem_count >= MAX_SEMAPHORES) {
        return -1; // No more semaphores available
    }

    semaphore_ptr sem = create_semaphore(name, value);
    semaphores[sem_count++] = sem;

    return 0; // Success
}

void syscall_create_semaphore_wrapper(argumentsStruct args) {
    char *name = (char *) args->r10;
    int value = (int) args->r9;
    args->r10 = syscall_create_semaphore(name, value);
}

semaphore_ptr syscall_open_semaphore(char *name) {
    for (int i = 0; i < sem_count; i++) {
        if (strcmp(semaphores[i]->name, name) == 0) {
            return semaphores[i];
        }
    }
    return NULL; // Semaphore not found
}

void syscall_open_semaphore_wrapper(argumentsStruct args) {
    char *name = (char *) args->r10;
    args->r10 = (uint64_t) syscall_open_semaphore(name);
}

void syscall_close_semaphore(semaphore_ptr sem) {
    for (int i = 0; i < sem_count; i++) {
        if (semaphores[i] == sem) {
            destroy_semaphore(sem);
            // Shift the remaining semaphores
            for (int j = i; j < sem_count - 1; j++) {
                semaphores[j] = semaphores[j + 1];
            }
            sem_count--;
            return;
        }
    }
}

void syscall_close_semaphore_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_close_semaphore(sem);
}

void syscall_semaphore_wait(semaphore_ptr sem) {
    semaphore_wait(sem);
}

void syscall_semaphore_wait_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_semaphore_wait(sem);
}

void syscall_semaphore_post(semaphore_ptr sem) {
    semaphore_post(sem);
}

void syscall_semaphore_post_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_semaphore_post(sem);
}

int pid(argumentsStruct args){
    return get_pid();
}

int pid_parent(argumentsStruct args){
    return get_pid_parent();
}

int set_status_syscall(argumentsStruct args){
    return set_status(args->rsi,args->rdx);
}

int change_priority_syscall(argumentsStruct args){
    return change_priority(args->rsi, args->rdx);
}

int kill_process_syscall(argumentsStruct args){
    return kill_process(args->rsi);
}

void show_all_processes(argumentsStruct args){
    show_processes();
}

// Array of syscall function pointers
void* (* syscalls[]) (argumentsStruct args) = {
        write,
        read,
        clean,
        setterBuffer,
        setFontSize,
        timer_wait,
        speaker_playSound,
        timeNow,
        putPix,
        updtScreen,
        foreGround,
        backGround,
        keyState,
        showRegisters,
        setPrintAnywhere,
        startS,
        stopS,
        execve,
        syscall_create_semaphore_wrapper,
        syscall_open_semaphore_wrapper,
        syscall_close_semaphore_wrapper,
        syscall_semaphore_wait_wrapper,
        syscall_semaphore_post_wrapper,
        show_all_processes,
        pid,
        pid_parent,
        set_status_syscall,
        change_priority_syscall,
        kill_process_syscall
};

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

void syscallDispatcher(argumentsStruct args) {
    if (args->rdi >= sizeofArr(syscalls)) {
        return;
    }
    // Ejecuta la syscall
    syscalls[args->rdi](args);
}
