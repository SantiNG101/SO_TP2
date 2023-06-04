/* sampleCodeModule.c */

#include <stdio.h>
#include "terminal.h"
#include "songs.h"
#include "pong.h"

extern void screenSetter();

int main() {

    printf("Welcome to Userland!\n");
    //  userLandStartTheme();
    unsigned char keepGoing = TRUE;

    while(keepGoing){
        setBackgroundColour(BLACK);
        setForegroundColour(WHITE);
        clearScreen(0);
        printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Terminal.\nPress 3 for exit.\n");
        char c = getChar();
        switch(c){
            case '1':
                playPong();
                break;
            case '2':
                terminalStart();
                break;
            case '3':
                keepGoing = FALSE;
                break;
        }
    }

	return 0;
}