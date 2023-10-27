#include "textures.h"

const Texture Textures[] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },  // EMPTY STATE,
    { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }  // ALL_OTHERS
};

void drawTexture(TextureType type, int x, int y){
    Texture * texture = Textures[type == EMPTY ? 0 : 1];

    // Draws texture on screen.
    int screenX = x * TEXTURE_WIDTH;
    int screenY = y * TEXTURE_HEIGHT;
    int pixIsPresent;

    for(int i = 0; i < TEXTURE_WIDTH; i++)
        for(int j = 0; j < TEXTURE_HEIGHT; j++){
            pixIsPresent = (*texture)[j] & (1 << (TEXTURE_WIDTH - i - 1));
            putPixel(screenX + i, screenY + j, pixIsPresent ? WHITE : BLACK);
        }
}