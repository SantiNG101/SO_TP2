//
// Created by ptorl on 5/17/2023.
//

#include "include/terminalHandler.h"
#include "include/keyboard_Handler.h"
#include "include/strings.h"
#include "include/time.h"
#include "include/lib.h"

#define CURRENT_FUNC_COUNT 5

const char * currentFuncs[] = {"time","clear","date","help","exit"};

const char * currentDescriptions[] = {0,0,0,0,0,"Shows the current time in GMT-3",0,0,0,"clears screen and resets position",
                               0,0,0,0,0,0,0,0,0,
                               0,0,"Displays current date.",0,0,0,0,0,0,
                               "Exits the bash",0,0,0,0,0,0,
                               "Provides different types of help depending on the amount of args passed.\nNo args mean display of current functions.\n1 Arg means getting description of a particular function",
                               0,
                               0,0,0,0,0,0,0,0,0,
                               0,0,0,0,0};

void help(char * token);
void showTime();
void showDate();

static unsigned char keepGoing = TRUE;

int terminalStart(){
    char* ptr = (char*)myMalloc(sizeof(char)*79);

    while(keepGoing){
        printf("$ ");

        scanf("%s",ptr);
        char *token = strtok(ptr," ");      //creo token con cmdline (modificable)
        // Process the command and execute actions accordingly

        if(!strcmp("help",token)){
            help(token);
        } else if(!strcmp("exit",token)){
            keepGoing = FALSE;
            printf("Exiting terminal.\n");
        } else if(!strcmp("time",token)){
            showTime();
        } else if(!strcmp("clear",token)){
            ncClear();
        } else if(!strcmp("date",token)){
            showDate();
        }else {
                printf("Command not found.\n"); //Cambiar por excepcion despues
        }

        printf("\n");

    }

    myFree(ptr);
    return 0;
}

void help(char * token){
    token = strtok(NULL," ");
    if(token == NULL){
        printf("Current functions are:\n");
        for(int i = 0; i < CURRENT_FUNC_COUNT; i++)
                printf("%s\n", currentFuncs[i]);

    } else {
        if(currentDescriptions[hashInRange(token,0,50)] != NULL) {
            printf("%s\n", currentDescriptions[hashInRange(token,0,50)]);
        } else {
            printf("Requested function does not exist.\n");
        }
    }
}

void showTime(){
    char time[50];
    timeToStr(time);
    printf("%s\n",time);
    return;
}

void showDate(){
    char date[50];
    dateToStr(date);
    printf("%s\n",date);
    return;
}

int64_t hashInRange(const char* str, uint64_t start, uint64_t end) {
    uint64_t hash = 33;

    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }

    return (hash % (end - start + 1)) + start;
}


/*
 * TOADD
 * showRegisters(); ---> Con interrupcion
 * Rtc();   --> Con interrupcion
 * self cleaning y que escriba en la ultima linea
 * generateXException(); --> genere div 0 y haga un seguimiento ?) y verifica su funcionamiento
 *
 */