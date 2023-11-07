#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <stdint.h>
#include <stdio.h>

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

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


/**
* @brief  Texture type definition
*/
// typedef enum { 
//     EMPTY = 0,
//     SNAKE_HEAD,
//     SNAKE_BODY,
//     SNAKE_TAIL,
//     FRUIT
// } TextureType;

typedef enum { 
    EMPTY = 0,

    // SNAKE_HEAD + DIRECTION
    SNAKE_HEAD = 1, // RIGHT
    SNAKE_HEAD_DOWN,
    SNAKE_HEAD_UP,
    SNAKE_HEAD_LEFT,

    SNAKE_BODY,

    // IF SNAKE BODY IS VERTICAL OR HORIZONTAL, THEN I KEEP THE SAME TAIL
    // ELSE I CHANGE IT USING. SNAKE_BODY
    SNAKE_TAIL, // RIGHT
    SNAKE_TAIL_DOWN,
    SNAKE_TAIL_UP,
    SNAKE_TAIL_LEFT,

    FRUIT,
 } TextureType;

// DIRECTION:
// typedef enum {
//     UP = 1,
//     DOWN,
//     LEFT,
//     RIGHT
// } Direction;


// EACH ROW HAS 8 BITS (1 BYTE), and each bit represents a pixel (1 if it's on, 0 if it's off)
typedef uint16_t Texture[TEXTURE_HEIGHT];

void drawTexture(TextureType textureType, int x, int y, uint64_t colour);

#endif