#include "terminal.h"

#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )

extern void opCode();
extern void terminalSetter();
extern void setBackgroundColour(uint32_t colour);
extern void setForegroundColour(uint32_t colour);
extern void showRegisters();


int64_t setBackground(int argc, char* argv[]);
int64_t setForeground(int argc, char* argv[]);
void runCommand(char *);
int64_t showTime(int argc, char* argv[]);
int64_t exit(int argc, char* argv[]);
int64_t showDate(int argc, char* argv[]);
int64_t help(int argc, char* argv[]);
int64_t divZero(int argc, char* argv[]);
int64_t beep(int argc, char* argv[]);
int64_t setFont(int argc, char* argv[]);
int64_t kill(int argc, char* argv[]);
int64_t nice(int argc, char* argv[]);
int64_t yield_shell(int argc, char* argv[]);
int64_t philos(int argc, char* argv[]);
int64_t filter(int argc, char* argv[]);
int64_t idle(int argc, char* argv[]);
int64_t finish_with_ctrlD(int argc, char* argv[]);
int is_vowel(char c);

void setDefault(){
    clearScreen(0);
    setBackgroundColour(BLACK);
    setForegroundColour(WHITE);
}

int64_t ps(int argc, char* argv[]);
int64_t echo(int argc, char* argv[]);
//int64_t jaime();


// struct para definir la lista de comandos a utilizar en la terminal
typedef struct {
    char name[20];
    char description[150];
    int64_t (*function)(int argc, char* argv[]);
} commandT;

typedef struct {
    char name[20];
    char description[150];
    void* (*function)(int, int);
} commandArgs;

typedef struct{
    char color[15];
    int hex;
}colour;

const commandT commands[] = {
                            {"help", "Provides a command list.", help},
                            {"time","Shows the current time in GMT-3",showTime},
                            {"clear","clears screen and resets position",terminalSetter},
                            {"date","Displays current date.",showDate},
                            {"exit","Exits the bash",exit},
                            {"bell","Outputs a Beep", beep},
                            {"66","Displays imperial march for starwars fans", imperialMarch},
                            {"mario","Displays mario bros theme song",marioTheme},
                            {"tetris","Displays tetris song",tetris},
                            {"storm","Displays song of storms zelda",songOfStorms},
                            {"background","Changes background to hexColour: ",setBackground},
                            {"foreground","Changes foreground to hexColour: ",setForeground},
                            {"div0","Shows how div 0 exception works",divZero},
                            {"opCode","Shows how opCode exception works",opCode},
                            {"font", "Sets the fontsize", setFont},
                            {"ps", "show all processes active in the system", ps},
                            {"SSR","Shows current saved registers. # Save registers pressing F11 #",showRegisters},
                            {"testprocess", "Test the processes", test_processes2},
                            {"kill", "kill selected processes", kill},
                            {"echo", "Print in shell", echo},
                            {"nice", "Print in shell", nice},
                            {"philos", "Print in shell", philos},
                            {"testpipes", "Print in shell", test_pipes},
                            {"filter", "Filter vowels", filter},
                            {"yield", "Set to rest the shell", yield_shell},
                            {"idle","busy wating to test ctrl+D", idle},
                            {"ctrlD","finish with ctrl+D", finish_with_ctrlD}
                            //{"jaime", "Who you gonna call? Jaime!", jaime}
                            };
/*
const commandArgs commands_args[] = {
                            {"pid", "Obtain the actual process pid", get_pid},
                            {"parentPid", "Obtain partents pid", get_pid_parent},
                            {"setStatus", "Change a process status ( BLOCKED=0 / READY=1 )", set_status},
                            {"kill", "kill an active process in the system", kill_process},
                            {"changePriority","change the priority of a process in the scheduling schema", change_priority}
                            };*/

#define BUFFER_SIZE 50
#define INSTRUCTION_SIZE 240    

typedef struct{
    char command[INSTRUCTION_SIZE];
    uint8_t size;
} instructionT;

typedef struct{
    instructionT data[BUFFER_SIZE];
    uint64_t readWriteIndex, size;
} bufferT;

// Lo dejo en una zona de memoria q
static bufferT buffer = { 0 };

static unsigned char keepGoing = TRUE;

/**
 * changeCommand 
 * 
 * Cambia el commando actual impreso en la pantalla con lo que hay en el buffer. 
 */
int changeCommand(int insNumber, char * current){
    if(insNumber >= buffer.size)
        return -1;                                          // Nunca debería entrar acá

    for(int i = 0; current[i]; i++){ putChar('\b'); }       // Borro los caracteres anteriores.
    strcpy(buffer.data[insNumber].command, current);
    puts(buffer.data[insNumber].command);                                          // Imprimo los actuales

    return buffer.data[insNumber].size;
}

/**
 * Guarda el comando en el buffer. (Si el readWriteIndex es mayor o igual al BUFFER_SIZE, entonces readWriteIndex se pone en 0),
 * y cambio read.
 * 
 */
int saveCommand(char * current){
    buffer.size += buffer.size <= BUFFER_SIZE;                          // Cambio el tamaño del buffer

    buffer.data[buffer.readWriteIndex].size = strlen(current);
    strcpy(current, buffer.data[buffer.readWriteIndex].command);        // Guardo el commando.

    buffer.readWriteIndex = (buffer.readWriteIndex + 1) % BUFFER_SIZE;  // Cambio la posición en la que leo y guardo.
    return 0;
}

char * getInstruction(char * ptr){
    uint8_t c;
    int j = buffer.readWriteIndex - 1;
    int i = 0;
    while((c = getChar()) != '\n' && i < INSTRUCTION_SIZE){
        switch(c){
            case 0x80:      // Arrow UP     (Pone los comandos más viejos)
                if(buffer.size > 0){
                    i = changeCommand(j, ptr);
                    j = (j + buffer.size - 1) % buffer.size;
                }
                break;
            case 0x81:      // Arrow DOWN   (Pone los comandos más nuevos)
                if(buffer.size > 0){
                    i = changeCommand(j, ptr);
                    j = (j + 1) % buffer.size;
                }
                break;
            case '\b':
                if(i != 0){
                    putChar(c);
                    --i;
                }
                ptr[i] = 0;
                break;
            default:
                putChar(c);
                ptr[i++] = c;
                break;
        }
    }
    putChar(c);
    ptr[i] = 0;

    return ptr;
}


int shell(int argc, char* argv[]){
    static char ptr[INSTRUCTION_SIZE] = { 0 };
    setDefault();
    keepGoing = TRUE;
    // llamado de syscall para setear al modo terminal pasandole el 0 que indica este modo
    terminalSetter();
    while(keepGoing){
        printf("$ ");

        getInstruction(ptr);
        saveCommand(ptr);

        char *token = strtok(ptr, " ");     //creo token con cmdline (modificable)
                                            //Process the command and execute actions accordingly
        runCommand(token);

        ptr[0] = 0; // Pongo en 0 el primer char para indicar que no hay ninguna instrucción.

        printf("\n");
    }

    setDefault();

    return 0;
}

// corre un comando, verifica con cual coincide del array de comandos y ejecuta su accion
// si no existe imprime comando incorrecto
void runCommand(char * cmd){
    for(int i = 0; i < SIZEOFARR(commands); i++){
        if(!strcmp(cmd, commands[i].name)) {
            // TODO: solo debe quedar la 4 y 25 aca
            if(i == 4 || i == 24 ){
                commands[i].function(NULL,NULL);
                putChar('\n');
                return;
            }

            // Nuevo código para manejar argumentos
            const int MAX_ARGS = 10; // Puedes ajustar esto según sea necesario
            char *args[MAX_ARGS + 2]; // +2 para el comando y el NULL final
            args[0] = commands[i].name; // El primer argumento es el nombre del comando
            int arg_count = 1;

            char *aux = strtok(NULL, " ");
            while(aux != NULL && arg_count < MAX_ARGS + 1) {
                if ( aux == "&" ){
                    break;
                }
                args[arg_count++] = aux;
                aux = strtok(NULL, " ");
            }
            args[arg_count] = NULL; // Terminar el arreglo con NULL

            putChar('\n');
            if(args[arg_count-1]=='&'){
                execve(getpid(), commands[i].function, arg_count, args, 0);
            }
            else {
                int pid = execve(getpid(), commands[i].function, arg_count, args, 1);
                wait_children(getpid());
            }
            //waitForProcess(pid);
            return;
        }

    }

/*
    for ( int i=0; i < SIZEOFARR(commands_args); i++ ){
        if ( !strcmp(cmd, commands_args[i].name) ){
            
        }
    }*/

        printf("Command not found.\n");
    return;
}

// imprime todas las opciones posibles si se lo llama solo
// imprime la funcionalidad si se lo acompaña con una funcion existente
int64_t help(int argc, char* argv[]){
    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("help requires only one argument.\n");
        exit_process(0);
        return 0;
    }

    if(token == NULL){
        printf("Current functions are:\n");
        for(int i = 0; i < SIZEOFARR(commands); i++){
            printf("%s - %s\n", commands[i].name, commands[i].description);
        }
        exit_process(0);
        return 0;
    }

    if(!strcmp(token,"help")){
        printf("Provides a list of functions or\nif an argument is passed\na brief description of the function passed as arg.\n");
        exit_process(0);
        return 0;
    }

    for( int i = 0; i < SIZEOFARR(commands); i++){
        if(!strcmp(token, commands[i].name)){
            printf("%s\n",commands[i].description);
            exit_process(0);
            return 0;
        }
    }

    printf("Desired function does not exist.\n");
    exit_process(-1);
    return -1;
}

// muestra el tiempo actual
int64_t showTime(int argc, char* argv[]){
    int hour,min,sec;
    getTime(&hour,&min,&sec);
    printf("%2d:%2d:%2d\n",hour,min,sec);
    exit_process(0);
    return 0;
}

// muestra la fecha actual
int64_t showDate(int argc, char* argv[]){
    int year,month,day;
    getDate(&day,&month,&year);
    printf("%2d/%2d/%d\n",day,month,year);
    return 0;
}

//funcion para salir del modo terminal
// unica funcion que no se deberia llamar como processo y debe correr exit_process()
int64_t exit(int argc, char* argv[]){
    keepGoing = FALSE;
    exit_process(0);
    return 0;
}

// funcion para dividir 
int divide(int x, int y){
    return x/y;
}

// funcion que fuerza la division por 0 para la excepcion
int64_t divZero(int argc, char* argv[]){
    divide(0,0);
    return 0;
}

//realiza el ruido
int64_t beep(int argc, char* argv[]){
    putChar('\a');
    return 0;
}

// array de colores que los relacionan con una descripcion
const colour colors[]= {{"black",BLACK},{"blue",BLUE},{"green",GREEN},
                        {"cyan",CYAN},{"red",RED},{"purple",PURPLE},
                        {"brown",BROWN},{"gray",GRAY},{"darkgray",DARK_GRAY},
                        {"lightblue",LIGHT_BLUE},{"lime",LIGHT_GREEN},{"lightcyan",LIGHT_CYAN},
                        {"lightred",LIGHT_RED},{"lightpurple",LIGHT_PURPLE},{"yellow",YELLOW},
                        {"white",WHITE}};

// funcion para aclarar colores no existentes
void nonExistentColor(){
    printf("Selected color is not available, please choose from the following list:\n");
    for(int j = 0; j < SIZEOFARR(colors)-1; j++){
        printf("%s, ",colors[j].color);
    }
    printf("%s \n\nExamples of usage: background cyan\n                   or\n                   foreground cyan",colors[SIZEOFARR(colors)-1].color);
    return;
}


// funcion para setear el background
int64_t setBackground(int argc, char* argv[]){

    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("Background requires only one argument.\n");
        return -1 ;
    }

    for(int i = 0; i < SIZEOFARR(colors); i++){
        if(!strcmp(colors[i].color,token)){
            setBackgroundColour(colors[i].hex);
            return 0;
        }
    }

    nonExistentColor();
    return 0;
}

// funcion para setear el color de las letras
int64_t setForeground(int argc, char* argv[]){

    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("Foreground requires only one argument.\n");
        return -1;
    }

    for(int i = 0; i < SIZEOFARR(colors); i++){
        if(!strcmp(colors[i].color,token)){
            setForegroundColour(colors[i].hex);
            return 0;
        }
    }

    nonExistentColor();
    return -1;
}

int64_t setFont(int argc, char* argv[]){
    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("Foreground requires only one argument.\n");
        return -1;
    }
    int size = atoi(token);

    if(size < 0 || size > 100){
        printf("Font size must be between 0 and 100.\n");
        return -1;
    }
    printf("Font size set to %d.\n", size);
    setFontSize(size);

    return 0;    
}

int64_t echo(int argc, char* argv[]) {
    printf("\n%d Parameters", argc);
    for(int i=0; i<argc; i++) {
        printf("%s", argv[i]);
    }
    exit_process(0);
    return 0;
}

int64_t ps(int argc, char* argv[]) {
    show_processes();
    exit_process(0);
    return 0;
}

int64_t kill(int argc, char* argv[]){
    if ( argc < 2 )
        exit_process(-1);
    int pid = atoi(argv[1]);
    int result = kill_process(pid);
    if(result == -1){
        exit_process(-1);
    }
    else {
        exit_process(0);
    }
    return 0;
}

int64_t yield_shell(int argc, char* argv[]){
    yield();
    return 0;
}

int64_t philos(int argc, char* argv[]){
    initPhyloReunion(argc, argv);
    exit_process(0);
    return 0;
}

int64_t nice(int argc, char* argv[]){
    if ( argc < 2 )
        exit_process(-1);
    int pid = atoi(argv[1]);
    lower_prio(pid);
}

int is_vowel(char c) {
    switch (c) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 'A': case 'E': case 'I': case 'O': case 'U':
            return 1;
        default:
            return 0;
    }
}

int64_t filter(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: filter <string1> <string2> ...\n");
        exit_process(1);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char *inputString = argv[i];

        char *ptr = inputString;
        while (*ptr != '\0') {
            if (!is_vowel(*ptr)) {
                putChar(*ptr);
            }
            ptr++;
        }

        if (i < argc - 1) {
            putChar(' '); // Separar los argumentos con un espacio
        }
    }

    putChar('\n');
    exit_process(0);
    return 0;
}

int64_t idle(int argc, char* argv[]){
    while(1){
        // busy waiting
        printf(argv[0]);
        printf(getpid());
        wait_time(2);        
    }
    return 0;
}

int64_t finish_with_ctrlD(int argc, char* argv[]){
    char c;
    while(c = getChar() != '\377'){
        // busy waiting
        putChar(c);       
    }
    exit_process(0);
    return 0;
}
