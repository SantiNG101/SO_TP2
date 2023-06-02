//
// Created by ptorl on 6/2/2023.
//

#include "include/interface.h"

void startInterface(){
    printf("Welcome to Userland!\n");
    userLandStartTheme();
    printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Terminal.\nPress 3 for exit.\n");
    unsigned char keepGoing = TRUE;

    while(keepGoing){
        char c = getChar();
        switch(c){
            case '1':
                break;
            case '2':
                terminalStart();
                printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Terminal.\nPress 3 for exit.\n");
                break;
            case '3':
                keepGoing = FALSE;
                break;
            default:
                break;
        }
    }
}

