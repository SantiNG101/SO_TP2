
#ifndef __PONG_H__
#define __PONG_H__

#include <stdio.h>

#define DEFAULT_BALL_RADIUS 7
#define DEFALUT_BAR_WIDTH 10
#define DEFALUT_BAR_HEIGHT 120

//reinicio posicion y dirección de la pelota
void resetBall(Ball *ball);

//reviso si colisiona con algún borde de la pantalla y en caso de requerirlo, suma puntaje o actualizar la velocidad
//de la pelota
void checkAndHandleWallCollision(Ball *ball, Player *player1, Player *player2);

//revisa si la pelota colisiona con la barra
int checkBarCollision(Ball ball, Bars bar);

//actualiza la velocidad de la pelota en base a la parte de la barra en la que colisionó
void barCollision(Ball *ball, Bars bar);

//actualiza la posición de la barra
void updateBar(Bars *bar, int direction);

//actualiza la posición de la pelota
void updateBall(Ball *ball, Player *player1, Player *player2);

//dibuja la barra
void drawBar(Bars bar);

//dibuja la pelota
void drawBall(Ball ball);

//dibuja el puntaje del juego
void drawScore(Game game);

//Interpreta las flechas y ws
void getInputPlaying();

#endif //__PONG_H__
