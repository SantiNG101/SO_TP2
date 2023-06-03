//
// Created by ptorl on 6/3/2023.
//

#include "figures.h"

const uint8_t numbersXL[][NUMBER_HEIGHT] = {
//  48 $30 'zero'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {	________,
                ________,
                __XXX___,
                _XX_XX__,
                XX___XX_,
                XX___XX_,
                XX_X_XX_,
                XX_X_XX_,
                XX___XX_,
                XX___XX_,
                _XX_XX__,
                __XXX___,
                ________,
                ________,
                ________,
                ________},
//  49 $31 'one'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                ___XX___,
                __XXX___,
                _XXXX___,
                ___XX___,
                ___XX___,
                ___XX___,
                ___XX___,
                ___XX___,
                ___XX___,
                _XXXXXX_,
                ________,
                ________,
                ________,
                ________},
//  50 $32 'two'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                _XXXXX__,
                XX___XX_,
                _____XX_,
                ____XX__,
                ___XX___,
                __XX____,
                _XX_____,
                XX______,
                XX___XX_,
                XXXXXXX_,
                ________,
                ________,
                ________,
                ________},
//  51 $33 'three'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                _XXXXX__,
                XX___XX_,
                _____XX_,
                _____XX_,
                __XXXX__,
                _____XX_,
                _____XX_,
                _____XX_,
                XX___XX_,
                _XXXXX__,
                ________,
                ________,
                ________,
                ________},
//  52 $34 'four'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                ____XX__,
                ___XXX__,
                __XXXX__,
                _XX_XX__,
                XX__XX__,
                XXXXXXX_,
                ____XX__,
                ____XX__,
                ____XX__,
                ___XXXX_,
                ________,
                ________,
                ________,
                ________},
//  53 $35 'five'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                XXXXXXX_,
                XX______,
                XX______,
                XX______,
                XXXXXX__,
                _____XX_,
                _____XX_,
                _____XX_,
                XX___XX_,
                _XXXXX__,
                ________,
                ________,
                ________,
                ________},
//  54 $36 'six'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                __XXX___,
                _XX_____,
                XX______,
                XX______,
                XXXXXX__,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                _XXXXX__,
                ________,
                ________,
                ________,
                ________},
//  55 $37 'seven'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                XXXXXXX_,
                XX___XX_,
                _____XX_,
                _____XX_,
                ____XX__,
                ___XX___,
                __XX____,
                __XX____,
                __XX____,
                __XX____,
                ________,
                ________,
                ________,
                ________},
//  56 $38 'eight'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                _XXXXX__,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                _XXXXX__,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                _XXXXX__,
                ________,
                ________,
                ________,
                ________},
//  57 $39 'nine'
//	width 8, bbx 0, bby -4, bbw 8, bbh 16
        {________,
                ________,
                _XXXXX__,
                XX___XX_,
                XX___XX_,
                XX___XX_,
                _XXXXXX_,
                _____XX_,
                _____XX_,
                _____XX_,
                ____XX__,
                _XXXX___,
                ________,
                ________,
                ________,
                ________}
};




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



uint8_t * numberBitmap( uint8_t number ){
    return numbersXL[number];
}

//Dibuja un numero xxl
void draw_numberXL(uint32_t x, uint32_t y, char character, uint32_t fontColor,uint32_t backgroundColor){
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
