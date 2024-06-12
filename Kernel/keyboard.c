//
// keyboard.c
//
#include <keyboard.h>

#define KEYBOARD_STATUS_PORT    0x64    //address status port
#define KEYBOARD_DATA_PORT      0X60    //address data port

/*
 *      specialKeys variable
 * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * | 0 |CAP|NUM|SCR|SHF|ALT|CTR| 0 |
 *
 * Reservo 8 bits para manipular el estado de las teclas especiales. Donde:
 *
 * CAP = CAP LOCK es el que deja en mayúscula
 * NUM = NUMBPAD LOCK habilita o deshabilita el uso del NumPad
 * SCR = SCROLL LOCK habilita o deshabilita el scroll
 *
 * SHF = Shift On|Off
 * ALT = ALT On|Off
 * CTR = CTRL On|Off
 *
 * En resumen, es un arreglo de flags.
 */
static uint8_t specialKeys = 0x0;
static char keyStates[256] = { 0 };

typedef struct blocked {
    uint32_t pid;
    struct blocked * next;
} * p_blocked;

static p_blocked first_blocked = NULL;

/* NOTAR QUE TIENEN EL MISMO NUMERO CORRESPONDIENTE AL BIT DE LA VARIABLE */
enum { CTR = 1, ALT, SHF, SCR, NUM, CAP };

static const char keyboard[6][16] = {{ '\f', '\x1B', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t' },
                                     { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', CTR, 'a', 's' },
                                     { 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHF, '\\', 'z', 'x', 'c', 'v' },
                                     { 'b', 'n', 'm', ',', '.', '/', SHF, '*', ALT, ' ', CAP, '\0', '\0', '\0', '\0', '\0' },
                                     { '\0', '\0', '\0', '\x0E', '\r', NUM, SCR, '7', 0x80 /* arrow up*/, '9', '-', '4', '5', '6', '+', '1' },
                                     { 0x81 /* arrow down*/, '3', '0', '.', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }};

const unsigned char shftKeyBoard[6][16] = {{ '\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t' },
                                           { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S' },
                                           { 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|', 'Z', 'X', 'C', 'V' },
                                           { 'B', 'N', 'M', '<', '>', '?', '\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0' },
                                           { '\0', '\0', '\0', '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.' },
                                           { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }};


/* EJEMPLO DE USO: getState(CTR) retorna si el CTRL está presionado o no */
#define getState(sk) ( specialKeys & (0x01 << sk) )

/* EJEMPLO DE USO: setState(CAP) cambia el estado del CAP LOCK */
#define setState(sk) ( specialKeys = specialKeys ^ (0x01 << sk))  // ^ es un XOR

/*
 * Esta MACRO parece magia oscura, pero lo que hace en realidad es buscar en la tabla,
 * sin importarle si se soltó la tecla, o no y fijarse que NO tenga los primeros 5 bits,
 * ya que eso implicaría que no es una specialKey.
 */
#define isSpecial(key) (!(((uint8_t) keyboard[(key >> 4) & 0x07][key & 0x0F]) & 0xF8))
#define isReleased(key) (key & 0x80)

static char currentBuff = 0;

void add_to_blocked(){
    p_blocked process = memalloc( sizeof(struct blocked) );
    process->pid = get_pid();
    process->next = NULL;

    if ( first_blocked == NULL ){
        first_blocked = process;
        return;
    }

    p_blocked aux = first_blocked;
    while( aux->next != NULL ){
        aux = aux->next;
    }

    aux->next = process;
    return;
}


uint32_t remove_blocked(){
    p_blocked aux = first_blocked;

    if ( aux == NULL )
        return 0;
    
    while( aux->next != NULL ){
        aux = aux->next;
    }

    return aux->pid;

}

void end_of_file(){
    currentBuff = -1;
    return;
}


void keyboardHandler(argumentsStruct args, uint64_t oldRSP){
    if((read_port(KEYBOARD_STATUS_PORT) & 0x01) == 0) return;   // Me aseguro que haya dato para leer.

    uint8_t data = read_port(KEYBOARD_DATA_PORT);

    if(isSpecial(data)){
        if(data == 0x57) {
            saveRegisterStatus(args, oldRSP);
        }
        char sp = keyboard[(data >> 4) & 0x07][data & 0x0F];

        // Me fijo si es un SpecialLock, si es un SpecialLock entonces me aseguro de que no pase dos veces por setState
        // Si no es un SpecialLock, entonces no hace falta ver si es released, porque se mantiene apretado
        // y luego se lo suelta.
        //
        // En otras palabras, como que el valor de SCR, NUM, CAP es de 4, 5 y 6 respectivamente. Es evidente que tienen
        // El bit de peso 2 activado. Mientras que los otros no lo tienen.
        if((sp & 0x04 && isReleased(data)) || !(sp & 0x04))
            setState(sp);
        return;
    }

    uint8_t keyCode = keyboard[data >> 4 & 0x07][data & 0x0F];
    if(getState(CAP))
        keyCode = toMayus(keyCode);
    if(getState(SHF))
        keyCode = shftKeyBoard[data >> 4 & 0x07][data & 0x0F];
    if(getState(CTR)){
        if ( keyCode == keyboard[2][0] ){
            end_of_file();
            return;
        }
        if ( keyCode == keyboard[2][14] ){
            kill_foreground();
            return;
        }

    }
    if((keyStates[keyCode] = (!isReleased(data))))
        currentBuff = keyCode;

    uint32_t pid = remove_blocked();
    if ( pid != 0 )
        set_status( pid, READY );
    

    return;
}

int getKeyState(int keyCode){
    return keyStates[keyCode];
}

int getC(){
    int c = currentBuff;
    if ( c == 0 ){
        add_to_blocked();
    }
    currentBuff = 0;

    return c;
}


