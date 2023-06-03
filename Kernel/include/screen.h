
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <videoDriver.h>

// inicializa los valores necesarios de la pantalla
void screen_Initialize();
// void printStrScreenFrmt( char * str, uint32_t font_color, uint32_t background_Color );
// setea el modo terminal para que imprima en la ultima linea
void setTerminalPrintingMode();
// mueve to el texto una para arriba cuando se queda sin espacio la pantalla
void scrollScreenUp();
// limpia la pantalla y la setea a color negro
void clearScreen();

/*
 * ------------------------------- Printing --------------------------------
 * Funciones: 
 *      print, printErrorStr, printDec, printHex, printBin, putCharScreen, 
 *      putCharScreenFrmt, backspace, tab, enter
 */

//
// functionName: print
// Description: Imprime un string
// params:
//      * (char *) str - String a imprimir 
//
void print(char * str);

//
// functionName: printErrorStr
// Description: Imprime un string con formato de error.
// params:
//      * (char *) str - String a imprimir
//
// imprime un string a pantalla con los formatos default
void printErrorStr(char * str);

//
// functionName: printDec
// Description: Imprime un número decimal
// params:
//      * (uint64_t) value - Decimal a imprimir.
//
void printDec(uint64_t value);

//
// functionName: printHex
// Description: Imprime un número hexadecimal
// params:
//      * (uint64_t) value - Hexadecimal a imprimir
//
void printHex(uint64_t value);

//
// functionName: printBin
// Description: Imprime un número binario
// params:
//      * (uint64_t) value - Binario a imprimir
//
void printBin(uint64_t value);

//
// functionName: printBin
// Description: Imprime un char a pantalla con el formato preestablecido
// params:
//      * (char) character - Caracter a imprimir.
//
void putCharScreen(char character);

//
// functionName: printBin
// Description: Imprime un char a pantalla con los formatos personalizados
// params:
//      * (char) character - Caracter a imprimir.
//      * (uint32_t) foreColour - Color del caracter.
//      * (uint32_t) backColour - Color de fondo del caracter.
//
void putCharScreenFrmt(char character, uint32_t foreColour, uint32_t backColour);

//
// functionName: backspace
// Description: Borra el char anterior
// params:
// 
void backspace();

//
// functionName: tab
// Description: Imprime un tab 
// params:
// 
void tab();

//
// functionName: enter
// Description: Pasa a la linea de abajo
// params:
// 
void enter();

/*
 * -------------------------------- Colores --------------------------------
 * Funciones: 
 *      setForegroundColour, setBackgroundColour, setPrintingColour
 */

//
// functionName: setForegroundColour
// Description: Cambia el color del texto.
// params:
//      * (uint32_t) colour - Color del texto a imprimir
//
void setForegroundColour(uint32_t colour);

//
// functionName: getForegroundColour
// Description: Obtiene el color del texto.
// params:
//
void getForegroundColour();

//
// functionName: setBackgroundColour
// Description: Cambia el color del fondo.
// params:
//      * (uint32_t) colour - Color del fondo a imprimir
//
void setBackgroundColour(uint32_t colour);

//
// functionName: setPrintingColour
// Description: Cambia el color del texto y del fondo.
// params:
//      * (uint32_t) foreground - Color del texto a imprimir
//      * (uint32_t) background - Color del fondo a imprimir
//
void setPrintingColour(uint32_t foreground, uint32_t background);

// funciones pong
// seteos iniciales
void gameMode();
// ya me pasan la direccion actualizada
void updatePongScreen(uint64_t * newGame);

#endif 