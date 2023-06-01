

#ifndef KERNEL_VIDEODRIVER_H
#define KERNEL_VIDEODRIVER_H

#include <stdint.h>
#include <font.h>

// asignacion de hexa a palabras

#define BLACK 0x000000
#define BLUE 0x0000AA
#define GREEN 0x00AA00
#define CYAN 0x00AAAA
#define RED 0xFF0000
#define PURPLE 0xAA00AA
#define BROWN 0xAA5500
#define GRAY 0xAAAAAA
#define DARK_GRAY 0x555555
#define LIGHT_BLUE 0x5555FF
#define LIGHT_GREEN 0x55FF55
#define LIGHT_CYAN 0x55FFFF
#define LIGHT_RED 0xFF5555
#define LIGHT_PURPLE 0xFF55FF
#define YELLOW	0xFFFF55
#define WHITE 0xFFFFFF

// definicion de colores default
#define COLOR_LETTER_DEFAULT WHITE
#define COLOR_BACKGROUND_DEFAULT BLACK


// inicializa el driver de pantalla
void vd_Initialize();
//imprime un pixel en pantalla
void putPixel(uint32_t hexColor, uint32_t x, uint32_t y);
//copia un pixel de un sector de la pantalla a otro
void copyPixel( uint32_t xfrom, uint32_t yfrom, uint32_t xto, uint32_t yto );
// dibuja un string en pantalla llamando al draw_char
void draw_string(uint32_t x, uint32_t y, char* input,uint32_t fontColor, uint32_t backgroundColor);
// dibuja un char en pantalla basandose en una font e imprimiendo con putpixel
void draw_char( uint32_t x, uint32_t y, char character, uint32_t fontColor, uint32_t backgroundColor );
// muestra la cantidad de pixeles que caben horizontalmente
uint16_t getHorizontalPixelCount();
// muestra la cantidad de pixeles que entran verticalmente
uint16_t getVerticalPixelCount();
// Actualiza la pantalla cuando se activa el doble buffer
void updateScreen();
// Seteo del estado del buffer, 1 activado 0 desactivado, default 0
void setDoubleBuffer(int activated);

#endif //KERNEL_VIDEODRIVER_H
