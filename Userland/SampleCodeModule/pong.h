
#ifndef __PONG_H__
#define __PONG_H__

#include <stdio.h>
#include "figures.h"
#include "songs.h"
#include <strings.h>
#define DEFAULT_BALL_RADIUS 7
#define DEFALUT_BAR_WIDTH 10
#define DEFALUT_BAR_HEIGHT 120
#define LIMIT_BAR_SPACE 15
#define MIDDLE_SCREEN (1024-30)/2
#define MARQUER_DISTANCE_X 15
#define MARQUER_DISTANCE_Y 15
#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define GAME_OVER 3
#define BAR_MOV 10
#define DEFAULT_BRADIUS 10
#define DEFAULT_BARSPEED 10
#define DEFAULT_BALLSPEED 8
#define DEFAULT_P1C BLUE
#define DEFAULT_P2C RED
#define DEFAULT_BC LIGHT_GREEN

void menuPong();
void pausePong();

#endif //__PONG_H__
