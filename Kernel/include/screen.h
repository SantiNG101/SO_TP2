
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <videoDriver.h>

// inicializa los valores necesarios de la pantalla
void screen_Initialize();
// imprime un string a pantalla con los formatos default
void printErrorStr( char* str );
// imprime a pantalla con los formatos personalizados
//void printStrScreenFrmt( char * str, uint32_t font_color, uint32_t background_Color );
// setea el modo terminal para que imprima en la ultima linea
void setTerminalPrintingMode();
// imprime un char a pantalla con el formato default
void putCharScreen( char character );
// imprime un char a pantalla con los formatos personalizados
void putCharScreenFrmt( char character, uint32_t font_color, uint32_t background_Color );
// mueve to el texto una para arriba cuando se queda sin espacio la pantalla
void scrollScreenUp();
// borra el char anterior
void backspace();
// imprime 2 char ' ' 
void tab();
// pasa a la linea de abajo
void enter();
// limpia la pantalla y la setea a color negro
void clearScreen();

// funciones pong
// seteos iniciales
void gameMode();
// ya me pasan la direccion actualizada
void updatePongScreen(uint64_t * newGame);

#endif 