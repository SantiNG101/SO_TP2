#include "include/terminalHandler.h"
#include "include/keyboard.h"
#include "include/strings.h"
#include "include/time.h"
#include "include/lib.h"
#include <stdio.h>
#include <screen.h>

#define CURRENT_FUNC_COUNT 5
#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )
const char * currentFuncs[] = {"time","clear","date","help","exit","69","mario","bell","darth"};

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
        setTerminal();
        printf("$ ");

        scanf("%s",ptr);
        char *token = strtok(ptr," ");      //creo token con cmdline (modificable)
        // Process the command and execute actions accordingly
        printf("\n");
        if(!strcmp("help",token)){
            help(token);
        } else if(!strcmp("exit",token)){
            keepGoing = FALSE;
            printf("Exiting terminal.\n");
        } else if(!strcmp("time",token)){
            showTime();
        } else if(!strcmp("clear",token)){
            clearScreen();
        } else if(!strcmp("date",token)) {
            showDate();
        }else if(!strcmp("69",token)) {
            printf("nice");
        }else if(!strcmp("bell",token)) {
            printf("\a");
        }else if(!strcmp("darth",token)) {
            printf("\"Did you ever hear the tragedy of Darth Plagueis the Wise?\"\n"
                   "\"No.\"\n"
                   "\"I thought not. It's not a story the Jedi would tell you. It's a Sith legend. Darth Plagueis... was a Dark Lord of the Sith so powerful and so wise, he could use the Force to influence the midi-chlorians... to create... life. He had such a knowledge of the dark side, he could even keep the ones he cared about... from dying.\"\n"
                   "\"He could actually... save people from death?\"\n"
                   "\"The dark side of the Force is a pathway to many abilities... some consider to be unnatural.\"\n"
                   "\"Wh– What happened to him?\"\n"
                   "\"He became so powerful, the only thing he was afraid of was... losing his power. Which eventually, of course, he did. Unfortunately, he taught his apprentice everything he knew. Then his apprentice killed him in his sleep. It's ironic. He could save others from death, but not himself.\"\n"
                   "\"Is it possible to learn this power?\"\n"
                   "\"Not from a Jedi.\"");
        }else if (!(strcmp("mario",token))){
            marioTheme();
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
        for(int i = 0; i < SIZEOFARR(currentFuncs); i++){
            printf("%s\n", currentFuncs[i]);
        }
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