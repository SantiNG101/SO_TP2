

#include <videoDriver.h>
#include <screen.h>

// tamaño de pantalla
// 1024 x 768


uint64_t screenWidht;
uint64_t screenHeight;
uint32_t current_cursor_pos_x = 0;
uint32_t current_cursor_pos_y = 0;

void screen_Initialize(){
    vd_Initialize();
    screenHeight = getVerticalPixelCount();
    screenWidht = getHorizontalPixelCount();
}


