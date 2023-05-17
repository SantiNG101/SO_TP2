#include "include/terminalHandler.h"
#include "include/keyboard_Handler.h"
#include "include/strings.h"

const char* CMD_LIST = {};

//Declaracion de funciones
void searchCmdTable(uint32_t  cmdHash, char ** args);
uint32_t parseCmd(char * lineCmd, char ** retArgs);
void interpretCmd(char** tokenArr);
void excecute();
void help();
void exitError();
unsigned long djb2_hash(const char *str);
// ----------------------------- //
static int keepGoing = TRUE;
static int error = FALSE;

int terminalStart(){
    char cmd[IN_LINE_BUFFER];
    char ** tokenArr;
    uint64_t currentHash;
    while(keepGoing){
        printf("$");
        scanf("%s",cmd);
        currentHash = parseCmd(cmd, tokenArr);      //verifica si es correcto el formato, si lo es => retorna puntero a un array con los tokens
        interpretCmd(tokenArr);
        printf("\n");
    }

    if(error){
        return -1;
    }
    return 0;
}

void terminalStop(){        //Hace que pare de recibir comandos y salga
    keepGoing = FALSE;
    return;
}

void exitError(){
    terminalStop();
    error = TRUE;
}

void searchCmdTable(uint32_t cmdHash, char** args){
    switch(cmdHash){
        default:
            printf("Mal parseado, no existe en la lista de comandos.");
            exitError();
            break;
    }
}

uint32_t parseCmd(char * lineCmd, char ** retArgs){

    return 0;
}

void interpretCmd(char** tokenArr){

}   //Interpreta y ejecuta

void help(){

}

/*
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 * this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
 * another version of this algorithm (now favored by bernstein)
 * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
 * (why it works better than many other constants, prime or not) has never been adequately explained.
 */
unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}