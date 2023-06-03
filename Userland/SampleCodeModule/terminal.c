#include "terminal.h"

#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )

extern void opCode();
extern void screenSetter();
extern void clearScreen();

void runCommand(char *);
void showTime();
void exit();
void showDate();
void help();
void divZero();
void beep();

typedef struct {
    char name[20];
    char description[100];
    void (*function)(void);
} commandT;

const commandT commands[] = {
                             {"help", "Provides a command list.", help},
                             {"time","Shows the current time in GMT-3",showTime},
                             {"clear","clears screen and resets position",clearScreen},
                             {"date","Displays current date.",showDate},
                             {"exit","Exits the bash",exit},
                             {"bell","Outputs a Beep", beep},
                             {"66","Displays imperial march for starwars fans", imperialMarch},
                             {"mario","Displays mario bros theme song",marioTheme},
                             {"tetris","Displays tetris song",tetris},
                             {"storm","Displays song of storms zelda",songOfStorms},
                             {"div0","Shows how div 0 exception works",divZero},
                             {"opCode","Shows how opCode exception works",opCode}
                            };

static unsigned char keepGoing = TRUE;

int terminalStart(){
    static const char ptr[240] = { 0 };
    // llamado de syscall para setear al modo terminal pasandole el 0 que indica este modo
    screenSetter(0);

    while(keepGoing){
        printf("$ ");

        scanf("%s", ptr);
        char *token = strtok(ptr, " ");     //creo token con cmdline (modificable)
                                            //Process the command and execute actions accordingly
        runCommand(token);

        printf("\n");
    }

    return 0;
}

void runCommand(char * cmd){
    for(int i = 0; i < SIZEOFARR(commands); i++){
        if(!strcmp(cmd, commands[i].name)) {
            if(i == 0){
                commands[i].function();
                putChar('\n');
                return;
            }
            char * aux = strtok(NULL, " ");
            if(aux != NULL){
                printf("This function does not accept arguments.\n");
                return;
            }

            commands[i].function();
            putChar('\n');
            return;
        }

    }
        printf("Comando no encontrado.\n");


    return;
}

void help(/*char * token*/){
    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("help requires only one argument.\n");
        return;
    }

    if(token == NULL){
        printf("Current functions are:\n");
        for(int i = 0; i < SIZEOFARR(commands); i++){
            printf("%s - %s\n", commands[i].name, commands[i].description);
        }
        return;
    }

    if(!strcmp(token,"help")){
        printf("Provides a list of functions or\nif an argument is passed\na brief description of the function passed as arg.\n");
        return;
    }

    for( int i = 0; i < SIZEOFARR(commands); i++){
        if(!strcmp(token, commands[i].name)){
            printf("%s\n",commands[i].description);
            return;
        }
    }

    printf("Desired function does not exist.\n");
}

void showTime(){
    int hour,min,sec;
    getTime(&hour,&min,&sec);
    printf("%d:%d:%d\n",hour,min,sec);
}

void showDate(){
    int year,month,day;
    getDate(&day,&month,&year);
    printf("%d/%d/%d\n",day,month,year);
}

void exit(){
    keepGoing = FALSE;
}

int divide(int x, int y){
    return x/y;
}

void divZero(){
    divide(0,0);
}

void beep(){
    putChar('\a');
}

/*
 * TOADD
 * showRegisters(); ---> Con interrupcion
 * Rtc();   --> Con interrupcion
 * self cleaning y que escriba en la ultima linea
 * generateXException(); --> genere div 0 y haga un seguimiento ?) y verifica su funcionamiento
 *
 */