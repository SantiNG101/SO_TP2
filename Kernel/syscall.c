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
#include <pipes.h>
#include "include/mm.h"

#define MAX_SEMAPHORES 100

static semaphore_ptr semaphores[MAX_SEMAPHORES];
static int sem_count = 0;
/*
 *  Argumentos de una función de SYSCALL
 */

int64_t read(argumentsStruct args){
    // This might read from keyboard buffer, print it and return it into the r10 Register.
    char*   data = (char *) args->r10;
    uint64_t len = (uint64_t) args->r9;
    char    c;
    if ( get_fd(0) != 0 )
        return read_pipe(get_fd(0),data,len);
    else{
        while(len){
            while ((c = getC()) == 0){
                
                set_status( get_pid(), BLOCKED );
                yield();
            }
            *data++ = c;
            len--;
        }
    }
    
    return 0;
}

int64_t write(argumentsStruct args){
    char * c = (char *) args->r10;
    int fd = get_fd(1);

    if ( fd == 1 ){
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
    }else {
            write_pipe(fd,c,args->r9);
    }

    return 0;
}

int64_t setterBuffer(argumentsStruct args){
    setDoubleBuffer(args->r10);
}

int64_t setFontSize(argumentsStruct args){
    setScale(args->r10);
    return 0;
}

int64_t timer_wait(argumentsStruct args){
    //wait(args->r10);
    return 0;
}

int64_t speaker_playSound(argumentsStruct args){
    beepCustom(args->r10, args->r9);
    return 0;
}

int64_t timeNow(argumentsStruct args){
    args->r10 = getTime();
    return 0;
}

int64_t clean(argumentsStruct args){
    clearScreen(args->r10);
    return 0;
}

//r10 x r9 y r8 color
int64_t putPix(argumentsStruct args){
    putPixel(args->r10,args->r9,args->r8);
    return 0;
}

int64_t updtScreen(argumentsStruct args){
    updateScreen();
    return 0;
}

int64_t foreGround(argumentsStruct args){
    setForegroundColour(args->r10);
    return 0;
}

int64_t backGround(argumentsStruct args){
    setBackgroundColour(args->r10);
    return 0;
}

int64_t keyState(argumentsStruct args){
    args->rsi = getKeyState(args->r10);
    return 0;
}

int64_t showRegisters(argumentsStruct args){
    printRegisterStatus();
    return 0;
}

int64_t setPrintAnywhere(argumentsStruct args){
    setPrintingMode(args->r10,args->r9,0);
    return 0;
}


// Starts a sound. With the given frq
int64_t startS(argumentsStruct args){
    playSound(args->r10);
    return 0;
}

// Stops the sound.
int64_t stopS(argumentsStruct args){
    stopSound();
    return 0;
}


int64_t syscall_create_semaphore(char *name, int value) {
    if (sem_count >= MAX_SEMAPHORES) {
        return -1; // No more semaphores available
    }

    semaphore_ptr sem = create_semaphore(name, value);
    semaphores[sem_count++] = sem;

    return 0; // Success
}

int64_t syscall_create_semaphore_wrapper(argumentsStruct args) {
    char *name = (char *) args->r10;
    int value = (int) args->r9;
    syscall_create_semaphore(name, value);
    return (int64_t) semaphores[sem_count-1];
}

semaphore_ptr syscall_open_semaphore(char *name) {
    for (int i = 0; i < sem_count; i++) {
        if (strcmp(semaphores[i]->name, name) == 0) {
            return semaphores[i];
        }
    }
    return NULL; // Semaphore not found
}

int64_t syscall_open_semaphore_wrapper(argumentsStruct args) {
    char *name = (char *) args->r10;
    return (uint64_t) syscall_open_semaphore(name);
}

int64_t syscall_close_semaphore(semaphore_ptr sem) {
    for (int i = 0; i < sem_count; i++) {
        if (semaphores[i] == sem) {
            destroy_semaphore(sem);
            // Shift the remaining semaphores
            for (int j = i; j < sem_count - 1; j++) {
                semaphores[j] = semaphores[j + 1];
            }
            sem_count--;
            return 0;
        }
    }
    return -1; // Semaphore not found
}

int64_t syscall_close_semaphore_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_close_semaphore(sem);
    return 0;
}

int64_t syscall_semaphore_wait(semaphore_ptr sem) {
    semaphore_wait(sem);
    return 0;
}

int64_t syscall_semaphore_wait_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_semaphore_wait(sem);
    return 0;
}

int64_t syscall_semaphore_post(semaphore_ptr sem) {
    semaphore_post(sem);
    return 0;
}

int64_t syscall_semaphore_post_wrapper(argumentsStruct args) {
    semaphore_ptr sem = (semaphore_ptr) args->r10;
    syscall_semaphore_post(sem);
    return 0;
}


// rdi, rsi, rdx, rcx, r8 and r9 in that order
int64_t execve(argumentsStruct args){
    return process_create(args->rsi, args->rdx, args->rcx, args->r8,args->r9);
}

int64_t get_fd_sys(argumentsStruct args){
    return get_fd(args->rsi);
}

int64_t set_fd_sys(argumentsStruct args){
    return set_fd(args->rsi, args->rdx, args->rcx);
}

int64_t pid(argumentsStruct args){
    return get_pid();
}

int64_t exit_syscall(argumentsStruct args){
    exit_process( args->rsi );
    return 0;
}

int64_t pid_parent(argumentsStruct args){
    return get_pid_parent();
}

int64_t set_status_syscall(argumentsStruct args){
    int result = set_status(args->rsi,args->rdx);
    if ( result == -1 )
        return (int64_t) -1;

    _sti();
    yield();

    return 0; 
}

int64_t change_priority_syscall(argumentsStruct args){
    return change_priority(args->rsi, args->rdx);
}

int64_t kill_process_syscall(argumentsStruct args){
    return kill_process(args->rsi);
}

int64_t show_all_processes(argumentsStruct args){
    show_processes();
    return 0;
}

int64_t yield_syscall(argumentsStruct args){
    yield();
    return 0;
}

int64_t open_pipe_sys(argumentsStruct args){
    return open_pipe_for_pid(args->rsi, args->rdx, args->rcx);
}

int64_t close_pipe_sys(argumentsStruct args){
    close_pipe_for_pid(args->rsi, args->rdx, args->rcx);
    return 0; 
}

int64_t nice_sys(argumentsStruct args){
    return change_priority(args->rsi,get_priority(args->rsi)+1);
}

int64_t notify_parent_sys(argumentsStruct args){
    notify_parent(args->rsi);
    return 0;
}

int64_t wait_children_sys(argumentsStruct args){
    wait_children(args->rsi);
    return 0;
}

int64_t malloc_sys(argumentsStruct args){
    return memalloc(args->rsi);
}

int64_t wait_sys(argumentsStruct args){
    return wait(args->rsi);
}


int64_t free_sys(argumentsStruct args){
    free(args->rsi);
    return 0;
}

int64_t mem_free_bytes(argumentsStruct args){
    return heap_free_bytes();
}

int64_t mem_used_bytes(argumentsStruct args){
    return heap_used_bytes();
}

int64_t keyboard_buffer_check_sys(argumentsStruct args){
    return buff_is_empty();
}


// Array of syscall function pointers
int64_t (* syscalls[]) (argumentsStruct args) = {
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
        kill_process_syscall,
        yield_syscall,
        exit_syscall,
        get_fd_sys,
        open_pipe_sys,
        close_pipe_sys,
        set_fd_sys,
        nice_sys,
        notify_parent_sys,
        wait_children_sys,
        malloc_sys,
        free_sys,
        wait_sys,
        keyboard_buffer_check_sys,
        mem_free_bytes,
        mem_used_bytes
};

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

int64_t syscallDispatcher(argumentsStruct args) {
    if (args->rdi >= sizeofArr(syscalls)) {
        return NULL;
    }
    // Ejecuta la syscall
    return syscalls[args->rdi](args);
}
