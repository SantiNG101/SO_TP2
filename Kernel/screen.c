

#include <screen.h>
#include <string.h>
// tamaño de pantalla
// 1024 x 768


uint64_t screenWidth;
uint64_t screenHeight;
uint32_t current_cursor_pos_x = 0;
uint32_t current_cursor_pos_y = 0;

void screen_Initialize(){
    vd_Initialize();
    screenHeight = getVerticalPixelCount();
    screenWidth = getHorizontalPixelCount();
}


// tabla de caracteres especiales
// tabla de relaciones con letras

/*
void printStrScreen( char* str ){
    
    int len = strlen(str);
    //version con draw char
    while(*str){
        putCharScreen(*str);
    }
    if ( (current_cursor_pos_x + len) < screenWidth )
        current_cursor_pos_x += len;
    else{
        len = current_cursor_pos_x + len - screenWidth;
        if ( current_cursor_pos_y )
        current_cursor_pos_y++;
        
        current_cursor_pos_x += len;
    }
}

void printStrScreenFrmt( char * str, uint32_t font_color, uint32_t background_Color ){

}
*/
void putCharScreen( char character ){
    bordersCheck();
    // imprimo el caracter en la pantalla
    draw_char( current_cursor_pos_x, current_cursor_pos_y, character,
     COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT );
    current_cursor_pos_x += CHAR_WIDTH;
}

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
    putCharScreen(' ');
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
    if ( current_cursor_pos_y <= screenHeight - CHAR_HEIGHT ){
        current_cursor_pos_y += CHAR_HEIGHT;
    }else{
        scrollScreenUp();
    }
    current_cursor_pos_x = 0; 
}

void scrollScreenUp(){
    // cada pixel lo copio en la de arriba
    for ( int i= 0; i < screenHeight - CHAR_HEIGHT; i++ ){ // y (altura)
        for( int j=0; j < screenWidth ; j++ ){ // x (ancho)
            copyPixel( j,i,j,i-CHAR_HEIGHT ); // copio al de arriba
        }
    }
    // imprimo en la ult linea espacios
    for ( int i= 0; i< screenWidth; i+=CHAR_WIDTH ){
        draw_char( i, screenHeight - CHAR_HEIGHT, ' ',
         COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT );
    }
    current_cursor_pos_x = 0;
    current_cursor_pos_y = screenHeight - CHAR_HEIGHT;
}

void clearScreen(){

    //imprimo en backgroundColor en toda la pantalla
    for ( int i= 0; i < screenHeight; i++ ){
        for( int j=0; j < screenWidth; j++ ){
            putPixel( j,i,COLOR_BACKGROUND_DEFAULT );
        }
    }
    // restauro el cursor al inicio
    current_cursor_pos_x = 0;
    current_cursor_pos_y = 0;
}

void bordersCheck(){
    if ( current_cursor_pos_x < screenWidth - CHAR_WIDTH +1 )
        return;

    current_cursor_pos_x = 0;
    if( current_cursor_pos_y < screenHeight - CHAR_HEIGHT +1){
            current_cursor_pos_y += CHAR_HEIGHT;
    }else
        scrollScreenUp();

}
  
