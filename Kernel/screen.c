

#include <screen.h>

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


void printStrScreen( char* str ){
    // version con string
    
    //version con draw char
    
    
    //strlen?? --> para mover el cursor


}

void printStrScreenFrmt( char * str, uint32_t font_color, uint32_t background_Color ){

}

void putCharScreen( char character ){
    bordersCheck();
    // imprimo el caracter en la pantalla
    draw_char( current_cursor_pos_x, current_cursor_pos_y, character,
     COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT );
    current_cursor_pos_x += CHAR_WIDTH;
}

void putCharScreenFrmt( char character, uint32_t font_color, uint32_t background_Color ){
    // imprimo el caracter en la pantalla
    draw_char( current_cursor_pos_x, current_cursor_pos_y, character,
     font_color, background_Color );
    current_cursor_pos_x +=CHAR_WIDTH;
}
void scrollScreenUp(){
    // cada pixel lo copio en la de arriba
    for ( int i= 0; i < screenHeight - CHAR_HEIGHT; i+=CHAR_HEIGHT ){
        for( int j=0; j < screenWidth ; j+= CHAR_WIDTH ){
            copyPixel( j,i,j,i-CHAR_HEIGHT );
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
  
