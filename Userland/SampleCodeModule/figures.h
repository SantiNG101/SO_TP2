//
// Created by ptorl on 6/3/2023.
//

#ifndef TPEARQUI_FIGURES_H
#define TPEARQUI_FIGURES_H


#include <stdio.h>
#include <stdint.h>

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

// imprime numeros mas grandes para el PONG especialmente
void draw_numberXL(uint32_t x, uint32_t y, char character, uint32_t fontColor,uint32_t backgroundColor);
// para el pong
void draw_Line(int x1, int y1, int x2, int y2, uint32_t color);
// dibuja un rectangulo relleno en pantalla
void draw_Rectangle(int x, int y, int width, int height, uint32_t color);
// Dibuja un circulo relleno en pantalla
void draw_CircleFilled(int centerX, int centerY, int radius, uint32_t color);
#endif //TPEARQUI_FIGURES_H
