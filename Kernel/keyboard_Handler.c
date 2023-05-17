//
// keyboard_Handler.c
//

#include <keyboard_Handler.h>
#include <strings.h>

#define KEYBOARD_STATUS_PORT 0x64       //address status port
#define KEYBOARD_DATA_PORT 0X60         //address data port
#define KEYBOARD_UPPER_BREAK 0x80       //break code address to compare the high bytes
#define KEYBOARD_BS 0XE                 //backspace address
#define KEYBOARD_LSHFT 0X2A             //left shift
#define K_LSHFT_BR 0XAA                 //left shift break code
#define K_RSHFT 0X36                    //r shift
#define K_RSHFT_BR 0XB6                 //r shift br code
#define K_TAB 0XF                       //tab code
#define K_ENTER 0X1C                    //enter code
#define ESCAPE 0x1
#define R_ARROW 0X4D
#define L_ARROW 0X4B
#define UP_ARROW 0X48
#define DOWN_ARROW 0X50
#define CTRL 0x1D
#define ALT 0X38
#define CTRL_BR 0x9D
#define ALT_BR 0xB8

/*
 *  MACROS
 */
#define isdigit(c) (c >= '0' && c <= '9')
#define isspace(c) (c == ' ')

const unsigned char kbdus[128] =
        {
                0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
                '9', '0', '-', '=', '\b',	/* Backspace */
                '\t',			/* Tab */
                'q', 'w', 'e', 'r',	/* 19 */
                't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
                0,			/* 29   - Control */
                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
                '\'', '`',   0,		/* Left shift */
                '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
                'm', ',', '.', '/',   0,				/* Right shift */
                '*',0,	/* Alt */' ',	/* Space bar */0,	/* Caps lock */0,	/* 59 - F1 key ... > */0,
                0,   0,   0,   0,   0,   0,   0,
                0,	/* < ... F10 */0,	/* 69 - Num lock*/0,	/* Scroll Lock */0,	/* Home key */0,	/* Up Arrow */0,	/* Page Up */'-',0,	/* Left Arrow */0,
                0,	/* Right Arrow */'+',0,	/* 79 - End key*/0,	/* Down Arrow */0,	/* Page Down */0,	/* Insert Key */
                0,	/* Delete Key */0,0,0,0,	/* F11 Key */0,	/* F12 Key */0,	/* All other keys are undefined */
        };

const unsigned char shiftedKbdus[128] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '(', ')', '{', '}', '\n', '\t', 0, '_', '+', '|', 0, '<', '>'};

static unsigned char isShift_Pressed = 0;  //lo uso para ver si el shift esta activo
static unsigned char isAlt_Pressed = 0;
static unsigned char isCtrl_Pressed = 0;
static unsigned char caps_lock_on = 0;

int scanf_int(int64_t * value_ptr);
int scanf_str(char * s_arg);
int scanf_hex(uint64_t * hexToDec);

int getChar() {
    int c = getC(KEYBOARD_STATUS_PORT, KEYBOARD_DATA_PORT);
    putChar(c);     //para que se pueda ver lo que el usuario inserto
    return c;
}

int getC(uint64_t status_port, uint64_t data_port){
    unsigned char status, key;
    unsigned char c;

    // Wait until input buffer is not empty
    do {
        status = read_port(status_port);
    } while ((status & 0x01) == 0);     // As long as there isnt pending input

    // Read key from port 0x60

    key = read_port(data_port);

    switch(key){
        case KEYBOARD_LSHFT:
        case K_RSHFT:
            isShift_Pressed = 1;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);
        case K_RSHFT_BR:
        case K_LSHFT_BR:
            isShift_Pressed = 0;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);
        case CTRL:
            isCtrl_Pressed = 1;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);
        case CTRL_BR:
            isCtrl_Pressed = 0;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);
        case ALT:
            isAlt_Pressed=1;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);
        case ALT_BR:
            isAlt_Pressed = 0;
            return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);

        default:
            //Los breaks codes los ignoro //
            if (key & KEYBOARD_UPPER_BREAK)
                return getC(KEYBOARD_STATUS_PORT,KEYBOARD_DATA_PORT);

            // Handle regular keys //
            if (isShift_Pressed) {
                c= shiftedKbdus[key];
            } else {
                c = kbdus[key];
            }
            return c;
    }
}

char * getString(void) {
    int i = 0;
    char * buff;
    char c;
    while ((c = getChar()) != '\n') {
        if (c != '\b') {
            buff[i++] = c;
        } else {
            if( i >= 1)
            buff[--i] = '\0';
        }
    }
    buff[i] = '\0';
    return buff;
}

/*
 *
 * El delimitador de cada scanf va a ser el '\n' indicando que se termino la linea
 * Los espacios vacios (white space seran ignorados siempre y cuando no sea un string)
 * Caso 'c' ==> lee un char
 * Caso 'd' ==> Lee un int de hasta 64 bit
 * Caso 's' ==> lee un str
 * Caso 'x' ==> lee un hexa de hasta-> lo pasa a dec
 *
 */

 
/*
 * Para que lo documente santi *-*
 */

int scanf(const char *format, ...) {
    va_list args;
    va_start(args,format);
    int flag_error = 0;

    while(*format != '\0' && !flag_error){
        if(*format == '%'){
            format++;
            switch(*format++){
                case 'd': {
                    int64_t *int_arg = va_arg(args,int64_t *);
                    if (scanf_int(int_arg) == -1) {
                        *int_arg = '\0';
                        flag_error = 1;
                        printf("Format error\n");
                    }
                    break;
                }
                case 'x': {
                    uint64_t *hex_arg = va_arg(args,uint64_t *);
                    if (scanf_hex(hex_arg) == -1) {
                        *hex_arg = '\0';
                        flag_error = 1;
                        printf("Format error\n");
                    }
                    break;
                }
                case 's': {
                    char *s_arg = va_arg(args,
                    char *);
                    scanf_str(s_arg);
                    break;
                }
                case 'c': {
                    char *c_arg = va_arg(args,char *);
                    *c_arg = getChar();
                    putChar('\n');
                    break;
                }
                default:
                    break;
            }
        } else {
            ++format;
        }
    }
    va_end(args);
    return 0;
}

//Func aux que procesa un String a Entero
int scanf_int(int64_t * value_ptr){
    int64_t value = 0;
    char c;
    int sign = 1;

    while ((c = getChar()) != '\n') {
        if (c == '-') {
            if(sign != -1) {
                sign = -1;
            } else {
                return -1;
            }
        } else if (isdigit(c)) {
            value = value * 10 + (c - '0');
        } else if (c == '\b') {
            value = value/10;
        } else {
            putChar('\n');
            break;
        }
    }

    *value_ptr = sign * value;
    return 1;
}

//Func aux que copia el str a un puntero
int scanf_str(char * s_arg){

    char * str = getString();
    strcpy(str, s_arg);

    return 0;
}

//Func aux que procesa un String (hex con fmt 0x o 0X) a entero
int scanf_hex(uint64_t *hexToDec) {
    uint64_t result = 0;
    char * hexStr = getString();

    //Verifico que entre en formato de hexa 0x // 0X
    if(!(hexStr[0] == '0' && (hexStr[1] == 'x' || hexStr[1] == 'X'))){
        return -1;
    }

    for (int i = 2; hexStr[i] != '\0'; i++) {
        result *= 16;
        if (hexStr[i] >= '0' && hexStr[i] <= '9') {
            result += hexStr[i] - '0';
        } else if (hexStr[i] >= 'a' && hexStr[i] <= 'f') {
            result += hexStr[i] - 'a' + 10;
        } else if (hexStr[i] >= 'A' && hexStr[i] <= 'F') {
            result += hexStr[i] - 'A' + 10;
        } else {
            // no es un hexa dec valido
            return -1;
        }
    }
    *hexToDec = result;
    return 0;
}






















