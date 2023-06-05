#include <screen.h>

// tamaño de pantalla
// 1024 x 768

void bordersCheck();

static uint64_t screenWidth;
static uint64_t screenHeight;
static uint32_t current_cursor_pos_x = 0;
static uint32_t current_cursor_pos_y = 0;

static char buffer[64] = { '0' };
static uint32_t foregroundColour = COLOR_LETTER_DEFAULT;
static uint32_t backgroundColour = COLOR_BACKGROUND_DEFAULT;

void screen_Initialize(){
    vd_Initialize();
    screenHeight = getVerticalPixelCount();
    screenWidth = getHorizontalPixelCount();
}

void setPrintingMode(uint32_t y, uint32_t x ,int bufferMode){
    current_cursor_pos_y = y;
    current_cursor_pos_x = x;
    setDoubleBuffer(bufferMode);
}

// setea el modo terminal para que imprima en la ultima linea
void setTerminalPrintingMode(){
    clearScreen(1);
    setPrintingMode(screenHeight - CHAR_HEIGHT, current_cursor_pos_x,0);
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
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

void setForegroundColour(uint32_t colour){ foregroundColour = colour; }
uint32_t getForegroundColour(){ return foregroundColour; }

void setBackgroundColour(uint32_t colour){ backgroundColour = colour; }

void setPrintingColour(uint32_t foreground, uint32_t background){
    setForegroundColour(foreground);
    setBackgroundColour(background);
}


void printStrScreenFrmt(char * str, uint32_t font_color, uint32_t background_Color ){
    while(*str){
        putCharScreenFrmt(*str++, font_color, background_Color);
    }
}

// Para imprimir un string normal.
void print(char * str){
    printStrScreenFrmt(str, foregroundColour, backgroundColour);
}

void printErrorStr(char* str){
    printStrScreenFrmt(str, RED, backgroundColour);
}

static void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

// Las diferentes base para imprimir.
void printDec(uint64_t value){ printBase(value, 10); }
void printHex(uint64_t value){ printBase(value, 16); }
void printBin(uint64_t value){ printBase(value, 2); }

void putCharScreen( char character ){ putCharScreenFrmt(character, foregroundColour, backgroundColour); }

void putCharScreenFrmt( char character, uint32_t font_color, uint32_t background_Color ){
    bordersCheck();
    // imprimo el caracter en la pantalla
    draw_char( current_cursor_pos_x, current_cursor_pos_y, character,
     font_color, background_Color );
    current_cursor_pos_x +=CHAR_WIDTH;
}

// retrocede 1 char limpiando su contenido
void backspace(){
    if ( current_cursor_pos_x == 0 ){
        current_cursor_pos_y-=CHAR_HEIGHT;
    }
    current_cursor_pos_x -= CHAR_WIDTH;
    putCharScreenFrmt(' ',WHITE,BLACK);
    current_cursor_pos_x -= CHAR_WIDTH;
}

// imprime 2 char ' ' 
void tab(){
    putCharScreen(' ');
    putCharScreen(' ');
    putCharScreen(' ');
    putCharScreen(' ');
}

// pasa a la linea de abajo
void enter(){
    if ( current_cursor_pos_y <= screenHeight - 2*2*CHAR_HEIGHT ){
        current_cursor_pos_y += CHAR_HEIGHT;
    }else{
        scrollScreenUp();
    }
    current_cursor_pos_x = 0; 
}

void scrollScreenUp(){
    // hago mem copy para modificar el frame buffer y paso si borro info o no
    modifyFrameBuffer(0);
    // imprimo en la ult linea espacios
    for ( int i= 0; i< screenWidth; i+=CHAR_WIDTH ){
        draw_char( i, screenHeight - CHAR_HEIGHT, ' ',
         COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT );
    }
    
    current_cursor_pos_x = 0;
    current_cursor_pos_y = screenHeight - CHAR_HEIGHT;
    
}
void clearScreen(uint8_t val){
    // hago mem copy para modificar el frame buffer y paso si borro info o no
    modifyFrameBuffer(1);
    // restauro el cursor al inicio
    current_cursor_pos_x = 0;
    current_cursor_pos_y = val ? screenHeight - CHAR_HEIGHT : 0;
}

void bordersCheck() {
    if (current_cursor_pos_x <= screenWidth - CHAR_WIDTH)
        return;

    current_cursor_pos_x = 0;
    if (current_cursor_pos_y <= screenHeight - 2 * CHAR_HEIGHT)
        current_cursor_pos_y += CHAR_HEIGHT;
    else
        scrollScreenUp();
}
