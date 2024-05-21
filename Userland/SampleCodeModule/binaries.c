
#include "binaries.h"
#include "terminal.h"
#include "songs.h"
#include "pong.h"
#include "snake.h"

void setDefault(){
    clearScreen(0);
    setBackgroundColour(BLACK);
    setForegroundColour(WHITE);
}

void shell(){
    setDefault();
            // userLandStartTheme();
    unsigned char keepGoing = 1;

    while(keepGoing){

        printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Snake.\nPress 3 for Terminal.\nPress 4 for exit.\n");
        char c = getChar();
        switch(c){
            case '1':
                menuPong();
                break;
            case '2':
                menuSnake();
                break;
            case '3':
                terminalStart();
                break;
            case '4':
                keepGoing = 0;
                break;
        }
        setDefault();
    }

	return;
}

void filosophers(){

}


/*

void (* functions[]) () = { shell, filosophers };

//
#define sizeofArr(arr) (sizeof(arr) / sizeof(arr[0]))

void functionDispatcher(char* function){
    enum { shell, filosophers };
    switch(function)
    
    // Ejecuta la syscall
    functions[args->rdi]((argumentsStruct) args);
}*/
