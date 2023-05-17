#include <naiveConsole.h>
#include <time.h>

extern uint32_t dVideo(uint8_t * puntero, char * str);
extern int getCurrentTime(void);
extern uint64_t   getCurrentRAX();
extern uint64_t   getCurrentRBX();
extern uint64_t   getCurrentRCX();
extern uint64_t   getCurrentRDX();
extern uint64_t   getCurrentRBP();
extern uint64_t   getCurrentRSP();
extern uint64_t   getCurrentRDI();
extern uint64_t   getCurrentRSI();
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncPrint(const char * string)
{
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}
void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

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

	return digits;
}


//Seccion putChar - puts - printf
int putChar(int c){
        switch(c){
            case '\n':
                handleEnter();
                break;
            case '\b':
                handleBackSpace();
                break;
            case '\t':
                handleTab();
                break;
            case '\0':
                break;
            case 27:            //caso del esc retorna su numero ascii
                break;
            default:
                ncPrintChar(c);
                break;
        }
        return c;
}

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

//Printf
int printf(const char * format, ...){
    va_list args;
    va_start(args, format);
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format++) {
                case 'd': {
                    int64_t int_arg = va_arg(args, int64_t);
                    ncPrintDec(int_arg);
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
                    ncPrintHex(hex_arg);
                    break;
                }
                default:
                    break;
            }
        } else {
            putChar(*format++);
        }
    }
    return 0;
    va_end(args);
}
//section video driver
void writeMsg(char * str){
   currentVideo = dVideo(currentVideo,str);
    ncNewline();
}

// Sectio kb
void handleBackSpace(){
    if(currentVideo > video) {
        currentVideo -= 2;      //voy para atras en el kernel
        *currentVideo = '\0';   //cambio el actual a un espacio
    }
}

void handleTab(){
    for(int i  = 0; i < 3; i++){
        *currentVideo = ' ';
        currentVideo += 2;
    }
}

void handleEnter(){
    ncNewline();
}

// Show register status
void showRegisterStatus(){
    printf("RAX: %x\tRBX: %x\nRCX: %x\tRDX: %x\nRSP: %x\tRBP: %x\nRSI: %x\tRDI: %x\n",
           getCurrentRAX(),getCurrentRBX(),getCurrentRCX(),getCurrentRDX(),getCurrentRSP(),getCurrentRBP(),
           getCurrentRSI(),getCurrentRDI());
}


