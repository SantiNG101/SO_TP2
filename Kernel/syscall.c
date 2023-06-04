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
/*
 *  Arguementos de una función de SYSCALL
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

void pongScreenUpdater(argumentsStruct args){
   // updatePongScreen(args->r10); // agregar los argumentos que vayan a utilizar
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

void (* syscalls[]) (argumentsStruct args) = { write, read, clean, setterBuffer, pongScreenUpdater,
 timer_wait, speaker_playSound, timeNow, putPix, updtScreen, foreGround, backGround, keyState,showRegisters,
 setPrintAnywhere };

#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

void syscallDispatcher(argumentsStruct args){
    if(args->rdi > sizeofArr(syscalls)){
        return;
    }
    // Ejecuta la syscall
    syscalls[args->rdi]((argumentsStruct) args);
}
