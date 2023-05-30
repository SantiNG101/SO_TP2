//
// keyboard.c
//
#include <keyboard.h>
#include <strings.h>

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

/* NOTAR QUE TIENEN EL MISMO NUMERO CORRESPONDIENTE AL BIT DE LA VARIABLE */
enum { CTR = 1, ALT, SHF, SCR, NUM, CAP };

static const char keyboard[6][16] = {{ '\f', '\x1B', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t' },
                                     { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', CTR, 'a', 's' },
                                     { 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHF, '\\', 'z', 'x', 'c', 'v' },
                                     { 'b', 'n', 'm', ',', '.', '/', SHF, '*', ALT, ' ', '\x0F', '\0', '\0', '\0', '\0', '\0' },
                                     { '\0', '\0', '\0', '\x0E', '\r', '\0', '\0', '7', '8', '9', '-', '4', '5', '6', '+', '1' },
                                     { '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' }};

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

static char buffer[64] = { 0 };
static char * currentBuff = buffer;

void keyboardHandler(){
    uint8_t data = read_port(KEYBOARD_DATA_PORT);
    
    if(isSpecial(data)){
        setState(keyboard[(data >> 4) & 0x07][data & 0x0F]);
        return;
    }

    if(isReleased(data))
        return;

    uint8_t keyCode = keyboard[data >> 4][data & 0x0F];
    if(getState(SHF))
        keyCode = shftKeyBoard[data >> 4][data & 0x0F];

    *currentBuff++ = keyCode;
    return;
}

int getC(){
    while((read_port(KEYBOARD_STATUS_PORT) & 0x01) == 0);
    keyboardHandler();

    int c = *--currentBuff;
    *currentBuff = 0;

    return c;
}

int getChar(){
    int c;
    while((c = getC()) == 0);

    putChar(c);        
    return c;
}

char * getString(char * buff, uint64_t len){
    int c;
    char * str = buff;
    while(len - 1 && (c = getChar()) != '\n'){
        switch(c){
            case '\b':
                if(str != buff)
                    *--str;
                *str = 0;
                len++;
                break;
            default:
                *str++ = c;
                len--;
                break;
        }
    }
}