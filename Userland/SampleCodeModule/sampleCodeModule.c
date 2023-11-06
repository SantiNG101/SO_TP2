/* sampleCodeModule.c */

#include <stdio.h>
#include "terminal.h"
#include "songs.h"
#include "pong.h"
#include "snake.h"

void setDefault(){
    clearScreen(0);
    setBackgroundColour(BLACK);
    setForegroundColour(WHITE);
}

int main() {
    setDefault();
            // userLandStartTheme();
    unsigned char keepGoing = TRUE;

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
                keepGoing = FALSE;
                break;
        }
        setDefault();
    }

	return 0;
}