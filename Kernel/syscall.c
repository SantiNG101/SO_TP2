//
// syscall.c
//
#include <naiveConsole.h>
#include <keyboard.h>
#include <stdint.h>
#include <screen.h>
#include <speaker.h>
#include <time.h>

/*
 *  Arguementos de una función de SYSCALL
 */
typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
} * argumentsStruct;

void write(argumentsStruct args);
void read(argumentsStruct args);
void modeSetter(argumentsStruct args);
void setterBuffer(argumentsStruct args);
void screenUpdater(argumentsStruct args);
void timer_wait(argumentsStruct args);
void speaker_playSound(argumentsStruct args);
void timeNow(argumentsStruct args);

void (* syscalls[]) (argumentsStruct args) = { write, read, modeSetter, setterBuffer, screenUpdater, timer_wait, speaker_playSound, timeNow };

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

void syscallDispatcher(argumentsStruct args){
    if(args->rdi > sizeofArr(syscalls)){
        return;
    }
    // Ejecuta la syscall
    syscalls[args->rdi]((argumentsStruct) args);
}

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

void modeSetter(argumentsStruct args){
    if ( args->r10 == 0 )
        setTerminalPrintingMode();
    else
        gameMode();
}

void setterBuffer(argumentsStruct args){
    setDoubleBuffer(args->r10);
}

void screenUpdater(argumentsStruct args){
    updateScreen();
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