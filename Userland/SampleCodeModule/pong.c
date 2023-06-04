#include "pong.h"
#include <stdlib.h>
// tamaño de pantalla
// 1024 x 768
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 1024
#define GAME_OVER 10
#define BAR_MOV 10

/* TODO Para la pelota hay que agregar un draw, updatePosition y detector de tocar la barra
    Mismo para las barras */

//TODO hacer clear screen (0,0) --> generico y setear terminal a abajo a la izq pal terminal mode
extern void terminalSetter();

// punto x e y de la pelota hacen referencia al centro de la pelota
typedef struct Ball{
    //posición actual
    int x;
    int y;

    //dirección de movimiento (velocidad) de la pelota
    int posx;
    int posy;

    int radius;
} * Ball;

// Punto y tomado de referencia es el de arriba de
typedef struct Bars {
    int y;
    int length;
} * Bars;


typedef struct Player{
    Bars bar;
    int score;
} * Player;


typedef struct Game{
    Player player1;
    Player player2;
    Ball ball;
} * Game;


static void updatePong(Game newGame){
    //clearScreen();
    terminalSetter();
    draw_Rectangle(LIMIT_BAR_SPACE,newGame->player1->bar->y,10,120,BLUE);
    draw_Rectangle(1024 - LIMIT_BAR_SPACE-10,newGame->player2->bar->y,10,120,RED);
    draw_Line (MIDDLE_SCREEN-1,0,MIDDLE_SCREEN-1,768,BLUE);
    draw_Line (MIDDLE_SCREEN-2,0,MIDDLE_SCREEN-2,768,BLUE);
    draw_Line (MIDDLE_SCREEN+2,0,MIDDLE_SCREEN+2,768,RED);
    draw_Line (MIDDLE_SCREEN+3,0,MIDDLE_SCREEN+3,768,RED);
    draw_number(MIDDLE_SCREEN-MARQUER_DISTANCE_X,MARQUER_DISTANCE_Y,newGame->player1->score,BLUE,COLOR_BACKGROUND_DEFAULT);
    draw_number(MIDDLE_SCREEN+MARQUER_DISTANCE_X+3*NUMBER_WIDTH,MARQUER_DISTANCE_Y,newGame->player2->score,RED,COLOR_BACKGROUND_DEFAULT);
    draw_CircleFilled(newGame->ball->x, newGame->ball->y,newGame->ball->radius, LIGHT_GREEN);
   updateScreen();
}

void resetBall(Ball ball){
    //reinicio la posición y dirección de la pelota
    ball->x = SCREEN_WIDTH/2;
    ball->y = SCREEN_HEIGHT/2;
    ball->posx = -ball->posx;
    ball->posy = 1;
}

void checkAndHandleWallCollision(Ball ball, Player player1, Player player2){
    //invierto la velocidad en y de la pelota
    ball->posy=-ball->posy;

    //reviso si la pelota colisionó en la parte de arriba o de abajo de la pantalla
    if(ball->y -ball ->radius <= 0 || ball->y + ball->radius >= SCREEN_HEIGHT-1){
        ball->posy = -ball->posy;
    }

    //reviso de la pelota colisionó con el borde derecho o izquierdo de la pantalla
    //de ser así, suma puntaje
    if (ball->x + ball->radius >= SCREEN_WIDTH-1){
        //player1 suma un punto y se reinicia la posición y velocidad
        player1->score++;
        resetBall(ball);
    }
    else if(ball->x - ball->radius <= 0){
        //player2 suma un punto y se reinicia la posición y velocidad
        player2->score++;
        resetBall(ball);
    }
}

int checkBarCollision(Ball ball, Bars bar){
    if(ball->x - ball->radius <= bar->length && ball->x + ball->radius >= 0){
        if(ball->y >= bar->y && ball->y <= bar->y + bar->length){
            return 1; //se detecta colision entre la pelota y la barra
        }
    }
    return 0; //no hay colision
}

void barCollision(Ball ball, Bars bar){
    //invierto la velocidad en la que se mueve la pelota
    ball->posx = -ball->posx;

    //ajusto la velocidad de la pelota dependiendo sobre que parte de la barra colisiona
    int barCenter = bar->y + bar->length / 2;
    int ballCenter = ball->y;
    int offset = ballCenter - barCenter;
    ball->posy = offset / 2;
}

void updateBar(Bars bar, int direction){
    //Actualizo posición de la barra
    bar->y += direction;
}

void updateBall(Ball ball, Player player1, Player player2){
    //actualizo la posición de la pelota
    ball->x += ball->posx;
    ball->y += ball->posy;

    //chequeo si hay colisiones con las barras
    if(checkBarCollision(ball, player1->bar) || checkBarCollision(ball, player2->bar)){
        barCollision(ball, player1->bar);
    }

    //chequeo si hay colision con los bordes de la pantalla
    checkAndHandleWallCollision(ball, player1,player2);

    //chequeo condiciones para fin del juego
    if(player1->score >= GAME_OVER || player2->score >= GAME_OVER){
        // reinicio puntaje y hago un resetBall
        player1->score=0;
        player2->score=0;
        resetBall(ball);
    }
}

//TODO CONTROL TECLADO FLECHAS EN SIMULTANEO
void getInputPlaying(Game game){
    if(getKeyState(80)) updateBar(game->player2->bar,-BAR_MOV);
    if(getKeyState(81)) updateBar(game->player2->bar, BAR_MOV);
    if(getKeyState('w'))updateBar(game->player1->bar,-BAR_MOV);
    if(getKeyState('s'))updateBar(game->player1->bar, BAR_MOV);
    
    return;
}

void playPong(){
    //p1 left     p2 right
    setBuffer(1);
    Game newGame = myMalloc(sizeof(Game));
    Ball ball = myMalloc(sizeof(Ball));
    Player p1= myMalloc(sizeof(Player));
    Player p2= myMalloc(sizeof(Player));
    Bars b1= myMalloc(sizeof(Bars));
    Bars b2= myMalloc(sizeof(Bars));

    newGame->ball = ball;
    newGame->player1 = p1;
    newGame->player2 = p2;

    p1->score = 0;
    p1->bar = b1;

    p2->score = 0;
    p2->bar = b2;

    //Comparten el y
    p1->bar->y = (768/2) - 60;
    p2->bar->y = (768/2) - 60;

    ball->radius = 7;
    ball->y = (768/2);
    ball->x = (1024-30)/2;
    ball->posx = 2;
    ball->posy = 2;

    updatePong(newGame);

    int p = 100;
    while(p1->score != 21 || p2->score != 21){
        getInputPlaying(newGame);
        updatePong(newGame);
    }

    myFree(b1);
    myFree(b2);
    myFree(p1);
    myFree(p2);
    myFree(ball);
    myFree(newGame);

    setBuffer(0);
    return;
}
