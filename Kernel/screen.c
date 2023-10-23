#include <screen.h>

// tamaño de pantalla
// 1024 x 768

void bordersCheck();

static uint64_t screenWidth;
static uint64_t screenHeight;
static uint32_t current_cursor_pos_x = 0;
static uint32_t current_cursor_pos_y = 0;
static int scale = 2;

static char buffer[64] = { '0' };
static uint32_t foregroundColour = COLOR_LETTER_DEFAULT;
static uint32_t backgroundColour = COLOR_BACKGROUND_DEFAULT;

// inicializa la pantalla, inicializando el driver de video y obeniendo la cantidad de pixeles de la pantalla en x e y
void screen_Initialize(){
    vd_Initialize();
    screenHeight = getVerticalPixelCount();
    screenWidth = getHorizontalPixelCount();
}

// funcion para poder imprimir en cualquier sector de la pantalla
void setPrintingMode(uint32_t y, uint32_t x ,int bufferMode){
    current_cursor_pos_y = y;
    current_cursor_pos_x = x;
    setDoubleBuffer(bufferMode);
}

void setScale(int newScale){
    scale = newScale;
}

// setea el modo terminal para que imprima en la ultima linea
void setTerminalPrintingMode(){
    clearScreen(1);
    setPrintingMode(screenHeight - CHAR_HEIGHT * scale, current_cursor_pos_x,0);
}

// funcion para el cambio de base
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

// setea el color de las letras
void setForegroundColour(uint32_t colour){ foregroundColour = colour; }
uint32_t getForegroundColour(){ return foregroundColour; }

// setea el color del fondo de las letras
void setBackgroundColour(uint32_t colour){ backgroundColour = colour; }

// funcion para el seteo de los colores de letra y fondo
void setPrintingColour(uint32_t foreground, uint32_t background){
    setForegroundColour(foreground);
    setBackgroundColour(background);
}

// imprime un string con un color y fondo especifico
void printStrScreenFrmt(char * str, uint32_t font_color, uint32_t background_Color ){
    while(*str){
        putCharScreenFrmt(*str++, font_color, background_Color);
    }
}

// Para imprimir un string normal.
void print(char * str){
    printStrScreenFrmt(str, foregroundColour, backgroundColour);
}

// imprimir en formato de error, color rojo letras blancas
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

// imprime char a pantalla con los colores default
void putCharScreen( char character ){ putCharScreenFrmt(character, foregroundColour, backgroundColour); }

// imprime char a pantalla con los colores especificados
void putCharScreenFrmt( char character, uint32_t font_color, uint32_t background_Color ){
    bordersCheck();
    // imprimo el caracter en la pantalla
    draw_scalable_char( current_cursor_pos_x, current_cursor_pos_y, character,
     font_color, background_Color, scale);
    current_cursor_pos_x +=CHAR_WIDTH*scale;
}

// retrocede 1 char limpiando su contenido
void backspace(){
    if ( current_cursor_pos_x == 0 ){
        current_cursor_pos_y-=CHAR_HEIGHT*scale;
    }
    current_cursor_pos_x -= CHAR_WIDTH*scale;
    putCharScreenFrmt(' ',WHITE,BLACK);
    current_cursor_pos_x -= CHAR_WIDTH*scale;
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
    if ( current_cursor_pos_y <= screenHeight - 2*2*CHAR_HEIGHT*scale ){
        current_cursor_pos_y += CHAR_HEIGHT*scale;
    }else{
        scrollScreenUp();
    }
    current_cursor_pos_x = 0; 
}

// funcion que permite mover el texto para arriba para liberar la ultima linea
void scrollScreenUp(){
    // hago mem copy para modificar el frame buffer y paso si borro info o no
    modifyFrameBuffer(0, scale);
    // imprimo en la ult linea espacios
    for ( int i= 0; i< screenWidth; i+=CHAR_WIDTH*scale ){
        draw_scalable_char( i, screenHeight - CHAR_HEIGHT*scale, ' ',
         COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT, scale);
    }
    
    current_cursor_pos_x = 0;
    current_cursor_pos_y = screenHeight - CHAR_HEIGHT*scale;
    
}

// limpia la pantalla 
void clearScreen(uint8_t val){
    // hago mem copy para modificar el frame buffer y paso si borro info o no
    modifyFrameBuffer(1, scale);
    // restauro el cursor al inicio
    current_cursor_pos_x = 0;
    current_cursor_pos_y = val ? screenHeight - CHAR_HEIGHT*scale : 0;
}

// chequeo de bordes para saber en que estado se ecuentran los current curson para sumarles si queda espacio o liberar el espacio
// para poder seguir escribiendo
void bordersCheck() {
    if (current_cursor_pos_x <= screenWidth - CHAR_WIDTH*scale)
        return;

    current_cursor_pos_x = 0;
    if (current_cursor_pos_y <= screenHeight - 2 * CHAR_HEIGHT*scale)
        current_cursor_pos_y += CHAR_HEIGHT*scale;
    else
        scrollScreenUp();
}
