#include "include/stdio.h"
char buff[255] = {0};
// Llama solo a getChar() y debe funcionar. 
char * getString(void){
    char c;
    char *str = buff;

    while((c = getChar()) != '\n'){
        if(c != '\b' || buff!= str)
            putChar(c);
        switch(c){
            case '\b':
                if(str != buff)
                    --str;
                *str = 0;
                break;
            default:
                *str++ = c;
                break;
        }
    }
    putChar(c);

    *str = 0;
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
static int scanf_int(int64_t *);
static int scanf_str(char *);
static int scanf_hex(uint64_t *);

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
                    char *s_arg = va_arg(args, char *);
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

int sscanf(const char * buffer, const char * format, ...){
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
                    char *s_arg = va_arg(args, char *);
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
static int scanf_int(int64_t * value_ptr){
    int64_t value = 0;
    char c;
    int sign = 1;

    while ((c = getChar()) != '\n') {
        if (c == '-') {
            //sign *= -1;
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
        putChar(c);
    }

    *value_ptr = sign * value;
    return 1;
}

//Func aux que copia el str a un puntero
static int scanf_str(char * s_arg){
    char * l = s_arg;
    strcpy(getString(),l);
    return 0;
}

//Func aux que procesa un String (hex con fmt 0x o 0X) a entero
static int scanf_hex(uint64_t *hexToDec) {
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


//Secion  Puts - Printf + funcs aux

//Putchar verison string
int puts(const char * string){
    int i;
    int flag;
    for (i = 0; string[i] != 0; i++) {
        flag = putChar(string[i]);
        if(flag < 0){
            return -1;
        }
    }

    return i;
}

//Auxiliar para convertir de hexa / dec a char [Es codigo dado por la catedra, modificado]
void uintToBase(uint64_t value, char * buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;

    //Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = '\0';

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return;
}

//Printf
int printf(const char * format, ...){
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            int padd = 0;
            while(*format >= '0' && *format <= '9'){ padd = padd*10 + *format++ - '0'; }
            switch (*format++) {
                case 'd': {
                    int64_t int_arg = va_arg(args, int64_t);
                    // char * toPrint = (char *)myMalloc(sizeof(int64_t)*5);
                    char toPrint[20] = { 0 };
                    uintToBase(int_arg, toPrint, 10);

                    while(padd > 20 || (padd > 0 && toPrint[padd - 1] == 0)){
                        putChar('0');
                        padd--;
                    }
                    puts(toPrint);
                                    // myFree(toPrint);
                    break;
                }
                case 's': {
                    char* s_arg = va_arg(args, char*);
                    puts(s_arg);
                    break;
                }
                case 'c': {
                    char c_arg = va_arg(args, int);
                    putChar(c_arg);
                    break;
                }
                case 'x': {
                    uint64_t hex_arg = va_arg(args, uint64_t);
                    // char * toPrint = (char *)myMalloc(sizeof(uint64_t)*5);
                    char toPrint[16] = { 0 };
                    uintToBase(hex_arg, toPrint,16);
                    puts(toPrint);
                                // myFree(toPrint);
                    break;
                }
                default:
                    break;
            }
        } else {
                putChar(*format++);
        }
    }
    va_end(args);
    return 0;
}

uint32_t malloc(uint32_t size) {
    malloc(size);
}

void free(uint32_t address) {
    free(address);
}