
#include <videoDriver.h>
#include <lib.h>

// realiza el chequeo de fuera de bordes retorna 1 si esta fuera, 0 si no.
int failBordersCheck( uint32_t x, uint32_t y );

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

uint16_t horizontalPixelCount;
uint16_t verticalPixelCount;
uint16_t numberOfColorBytes;
uint64_t bufferlen = 1024*768*3;
// genero un buffer con las dimensiones seteadas a mano
// sirve para hacer la transicion mas suave principalmente en el pong
static uint8_t  buffer[ 1024*768*3 ];
int doubleBuffer = 0;

void vd_Initialize(){
	horizontalPixelCount = VBE_mode_info->width;
	verticalPixelCount = VBE_mode_info->height;
	numberOfColorBytes = VBE_mode_info->bpp/8;
	if ( doubleBuffer != 0 )
		memset(buffer, 0, bufferlen);
}


void updateScreen(){
	if ( doubleBuffer != 0 )
		memcpy(VBE_mode_info->framebuffer,buffer,bufferlen);
}

void setDoubleBuffer(int activated){
	doubleBuffer = activated;
}

/*
void putPixel(uint8_t r, uint8_t g, uint8_t b, uint32_t x, uint32_t y) {
	uint8_t * videoPtr = VBE_mode_info->framebuffer;
	int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
	videoPtr[offset] = b;
	videoPtr[offset+1] = g;
	videoPtr[offset+2] = r;
}
*/
void putPixel( uint32_t x, uint32_t y, uint32_t hexColor) {
	// setei el offset en el punto pasado
	uint32_t offset = VBE_mode_info->pitch*y + x*numberOfColorBytes;
	// veo si esta activado el doble buffer 
	if ( doubleBuffer ){
		buffer[offset] = hexColor & 0xFF;
		buffer[offset+1] = (hexColor >> 8) & 0xFF;
		buffer[offset+2] = (hexColor >> 16) &  0xFF;
		return;
	}else{
		uint8_t * screen = (uint8_t *) ((uint64_t) (VBE_mode_info->framebuffer));
		screen[offset] = hexColor & 0xFF;
		screen[offset+1] = (hexColor >> 8) & 0xFF;
		screen[offset+2] = (hexColor >> 16) &  0xFF;
	}
}


// necesario para cuando se haga la subida de pantalla
// hace copia de memoria para cambiar el estado de los pixeles de la pantalla de lugar
void modifyFrameBuffer( int mode){
	/*
	// creo el offset de form
	uint32_t offsetFrom = VBE_mode_info->pitch*yfrom + xfrom*numberOfColorBytes;
	// creo el offset de to
	uint32_t offsetTo = VBE_mode_info->pitch*yto + xto*numberOfColorBytes;
	// paso uno al otro
	if ( doubleBuffer ){
		buffer[offsetTo] = buffer[offsetFrom];
		buffer[offsetTo+1] = buffer[offsetFrom+1];
		buffer[offsetTo+2] = buffer[offsetFrom+2];
		return;
	}else{
		uint8_t * screen = (uint8_t *) ((uint64_t) (VBE_mode_info->framebuffer));
		screen[offsetTo] = screen[offsetFrom];
		screen[offsetTo+1] = screen[offsetFrom+1];
	 	screen[offsetTo+2] = screen[offsetFrom+2];
	}
	*/
	// hago mem copy de memoria
	if ( doubleBuffer ){
		if (mode){
		memset(buffer,0,bufferlen);
		return;
		}
		memcpy(buffer,buffer+ CHAR_HEIGHT*1024*3,bufferlen - CHAR_HEIGHT*1024*3 );
		return;
	}
	if (mode){
		memset(VBE_mode_info->framebuffer,0,bufferlen);
		return;
	}
	memcpy(VBE_mode_info->framebuffer,VBE_mode_info->framebuffer+ CHAR_HEIGHT*1024*3,
	bufferlen - CHAR_HEIGHT*1024*3 );
}
	

// chequear esta funcion pues el string se puede cortar en pantalla
void draw_string(uint32_t x, uint32_t y, char* input,uint32_t fontColor,
				 uint32_t backgroundColor) {

    while(*input) {
        draw_char(x,y,input[0],fontColor, backgroundColor);
        x += CHAR_WIDTH;
		if ( failBordersCheck(x, y)){
			y+=CHAR_HEIGHT;
			x = 0;
		}
        input++;
    }
}


void draw_char( uint32_t x, uint32_t y, char character, uint32_t fontColor,
				 uint32_t backgroundColor ){

	if ( failBordersCheck(x, y)){
		return;
	}

	uint32_t auxx = x, auxy = y;
	char pixIsPresent = 0;

	uint8_t* font_char = charBitmap(character);

	for(int i = 0; i < CHAR_HEIGHT; i++){
		for(int j = 0; j < CHAR_WIDTH; j++){
			// Veo si tengo que imprimir el pixel o no, +2 para que imprima bien con el correcto decalaje de bits
			// No utlizo los ultimos 2 bits porque no son del tamaño asignado
			pixIsPresent = (1 << (CHAR_WIDTH + 2 - j)) & font_char[i];
			if ( pixIsPresent )
				putPixel( auxx, auxy, fontColor );
			else
				putPixel( auxx, auxy, backgroundColor );
				auxx++;
		}
		auxx = x;
		auxy++;
	}
}

void draw_numberXL(uint32_t x, uint32_t y, char character, uint32_t fontColor,
				 uint32_t backgroundColor){
	if ( failBordersCheck(x, y)){
		return;
	}

	uint32_t auxx = x, auxy = y;
	char pixIsPresent = 0;

	uint8_t* font_char = numberBitmap(character);

	for(int i = 0; i < NUMBER_HEIGHT; i++){
		for(int j = 0; j < NUMBER_WIDTH; j++){
			// Veo si tengo que imprimir el pixel o no, +2 para que imprima bien con el correcto decalaje de bits
			// No utlizo los ultimos 2 bits porque no son del tamaño asignado
			pixIsPresent = (1 << (NUMBER_WIDTH -1 - j)) & font_char[i];
			if ( pixIsPresent )
				putPixel( auxx, auxy, fontColor );
			else
				putPixel( auxx, auxy, backgroundColor );
				auxx++;
		}
		auxx = x;
		auxy++;
	}



}


int failBordersCheck( uint32_t x, uint32_t y ){
	//chequeo los limites (es - o / comparando con la cantidad de espacio del char en los bordes)
	if ( x < 0 || x > horizontalPixelCount - CHAR_WIDTH || y < 0
				|| y > verticalPixelCount - CHAR_HEIGHT){
		return 1;
	}
	return 0;
}

uint16_t getHorizontalPixelCount(){
	return horizontalPixelCount;
}
uint16_t getVerticalPixelCount(){
	return verticalPixelCount;
}





// funciones para el pong

// dibuja una linea en pantalla
void draw_Line(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int stepX = (dx < 0) ? -1 : 1;
    int stepY = (dy < 0) ? -1 : 1;

    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    int doubleDx = dx << 1;
    int doubleDy = dy << 1;

    int x = x1;
    int y = y1;
    int error;

    if (dx > dy) {
        error = doubleDy - dx;

        while (x != x2) {
            putPixel(x, y, color);

            if (error > 0) {
                y += stepY;
                error -= doubleDx;
            }

            error += doubleDy;
            x += stepX;
        }
    } else {
        error = doubleDx - dy;

        while (y != y2) {
            putPixel(x, y, color);

            if (error > 0) {
                x += stepX;
                error -= doubleDy;
            }

            error += doubleDx;
            y += stepY;
        }
    }

    putPixel(x2, y2, color);
}

// dibuja un rectangulo relleno en pantalla
void draw_Rectangle(int x, int y, int width, int height, uint32_t color) {
    int xEnd = x + width;
    int yEnd = y + height;

	for ( int i=y; i <= yEnd; i++ ){
		draw_Line(x,i,xEnd,i,color);
	}
}

// Dibuja un circulo relleno en pantalla
void draw_CircleFilled(int centerX, int centerY, int radius, uint32_t color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        // Dibuja una línea horizontal entre los puntos (x - y) y (x + y)
        for (int i = centerX - y; i <= centerX + y; i++) {
            putPixel(i, centerY + x, color);
            putPixel(i, centerY - x, color);
        }
        // Dibuja una línea horizontal entre los puntos (x - y) y (x + y)
        for (int i = centerX - x; i <= centerX + x; i++) {
            putPixel(i, centerY + y, color);
            putPixel(i, centerY - y, color);
        }

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }

        x++;
    }
}
