
#include <videoDriver.h>
#include <lib.h>

// realiza el chequeo de fuera de bordes retorna 1 si esta fuera, 0 si no.
int failBordersCheck( uint32_t x, uint32_t y, uint8_t OffsetX, uint8_t OffsetY);

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


// inicializa los aspectos basico de la pantalla
void vd_Initialize(){
	horizontalPixelCount = VBE_mode_info->width;
	verticalPixelCount = VBE_mode_info->height;
	numberOfColorBytes = VBE_mode_info->bpp/8;
	if ( doubleBuffer != 0 )
		memset(buffer, 0, bufferlen);
}

// funcion que actualiza la pantalla si se esta utiilzando el double buffer
void updateScreen(){
	if ( doubleBuffer != 0 )
		memcpy(VBE_mode_info->framebuffer,buffer,bufferlen);
}

// activa o desactiba el doble buffer para que se vea mas suavizado el cambio de pantalla
void setDoubleBuffer(int activated){
	doubleBuffer = activated;
}

// imprime un pixel en pantalla
void putPixel( uint32_t x, uint32_t y, uint32_t hexColor) {
    if ( failBordersCheck(x, y, 0, 0)){
        return;
    }

	// seteo el offset en el punto pasado
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


// hace copia de memoria para cambiar el estado de los pixeles de la pantalla de lugar
void modifyFrameBuffer( int mode){
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
	

// imprime un string en pantalla
void draw_string(uint32_t x, uint32_t y, char* input,uint32_t fontColor,
				 uint32_t backgroundColor) {

    while(*input) {
        draw_char(x,y,input[0],fontColor, backgroundColor);
        x += CHAR_WIDTH;
		if ( failBordersCheck(x, y, CHAR_WIDTH, CHAR_HEIGHT)){
			y+=CHAR_HEIGHT;
			x = 0;
		}
        input++;
    }
}

// imprime un char en pantalla basandose en un charBitmap en font
void draw_char( uint32_t x, uint32_t y, char character, uint32_t fontColor,
				 uint32_t backgroundColor ){
	if ( failBordersCheck(x, y, CHAR_WIDTH, CHAR_HEIGHT)){
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


int failBordersCheck( uint32_t x, uint32_t y, uint8_t OffsetX, uint8_t OffsetY){
	//chequeo los limites (es - o / comparando con la cantidad de espacio del char en los bordes)
	return  x < 0 || x > horizontalPixelCount - OffsetX || y < 0 || y > verticalPixelCount - OffsetY;
}

uint16_t getHorizontalPixelCount(){
	return horizontalPixelCount;
}

uint16_t getVerticalPixelCount(){
	return verticalPixelCount;
}



