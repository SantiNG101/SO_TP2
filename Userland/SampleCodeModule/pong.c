#include "pong.h"
#include <stdlib.h>
// tamaño de pantalla
// 1024 x 768
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define GAME_OVER 5
#define BAR_MOV 10

static int keepGoing = 1;

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
    int x;
    int y;
    int width;
    int height;
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
    draw_Rectangle(newGame->player1->bar->x, newGame->player1->bar->y, newGame->player1->bar->width, newGame->player1->bar->height, BLUE);
    draw_Rectangle(newGame->player2->bar->x, newGame->player2->bar->y, newGame->player2->bar->width, newGame->player2->bar->height,RED);
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
    ball->x = (SCREEN_WIDTH-30)/2;
    ball->y = SCREEN_HEIGHT/2;
    ball->posx = -ball->posx;
    ball->posy = 2;
}

void checkAndHandleWallCollision(Ball ball, Player player1, Player player2){

    //reviso si la pelota colisionó en la parte de arriba o de abajo de la pantalla
    if(ball->y - ball->radius <= 0 || ball->y + ball->radius >= SCREEN_HEIGHT-1){
        ball->posy = -ball->posy;
        return;
    }
    //reviso de la pelota colisionó con el borde derecho o izquierdo de la pantalla
    //de ser así, suma puntaje
    if (ball->x + ball->radius >= SCREEN_WIDTH-1){
        //player1 suma un punto y se reinicia la posición y velocidad
        player1->score++;
        resetBall(ball);
        return;
    }

    if(ball->x - ball->radius <= 0){
        //player2 suma un punto y se reinicia la posición y velocidad
        player2->score++;
        resetBall(ball);
    }
}

int isOnRange(int left, int right, int value){
    return value >= left && value <= right;
}

int checkBarCollision(Ball ball, Bars bar){
    int leftCollision = isOnRange(bar->x, bar->x + bar->width, ball->x + ball->radius);
    int rightCollision = isOnRange(bar->x, bar->x + bar->width, ball->x - ball->radius);
    int yCollision = isOnRange(bar->y, bar->y + bar->height, ball->y);
    
    return (leftCollision || rightCollision) && yCollision;
}

void barCollision(Ball ball, Bars bar){
    //invierto la velocidad en la que se mueve la pelota
    ball->posx = -ball->posx;

    //ajusto la velocidad de la pelota dependiendo sobre que parte de la barra colisiona
    int barCenter = bar->y + (bar->height / 2);
    int ballCenter = ball->y;
    int offset = ballCenter - barCenter;
    ball->posy = offset / 2;
}

void updateBar(Bars bar, int direction){
    //Actualizo posición de la barra
    bar->y += direction;
}

void updateBall(Ball ball, Player player1, Player player2){
    ball->x += ball->posx;
    ball->y += ball->posy;
    //chequeo si hay colisiones con las barras
    if(checkBarCollision(ball, player2->bar)){
        barCollision(ball, player2->bar);
        return;
    }

    if(checkBarCollision(ball, player1->bar)){
        barCollision(ball, player1->bar);
        return;
    }
      
    //chequeo si hay colision con los bordes de la pantalla
    checkAndHandleWallCollision(ball, player1, player2);

    //chequeo condiciones para fin del juego
    // if(player1->score >= GAME_OVER || player2->score >= GAME_OVER){
    //     // reinicio puntaje y hago un resetBall
    //     player1->score=0;
    //     player2->score=0;
    //     resetBall(ball);
    // }
}

//TODO CONTROL TECLADO FLECHAS EN SIMULTANEO //
void getInputPlaying(Game game){
    if(getKeyState(0x80)) updateBar(game->player2->bar,-BAR_MOV);
    if(getKeyState(0x81)) updateBar(game->player2->bar, BAR_MOV);
    if(getKeyState('w'))updateBar(game->player1->bar,-BAR_MOV);
    if(getKeyState('s'))updateBar(game->player1->bar, BAR_MOV);
    if(getKeyState(0x1B))pausePong();
    
    return;
}

Player buildPlayer(int barX){
    Player player = myMalloc(sizeof(Player));
    Bars bar = player->bar = myMalloc(sizeof(Bars));

    player->score = 0;

    bar->width = 30;
    bar->height = 120;
    bar-> y = (768/2) - 60;
    bar->x = barX;

    return player;
}

Ball buildBall(int radius, int x, int y, int posx, int posy){
    Ball ball = myMalloc(sizeof(Ball));

    ball->x = x;
    ball->y = y;
    ball->posx = posx;
    ball->posy = posy;
    ball->radius = 10;

    return ball;
}


void pausePong(){
    // Imprimo el menú
    setBuffer(0);

    printf("PAUSA: \n");
    printf("Para continuar presione [ P ]\n");
    printf("Para cerrar el juego presione [ C ]");

    // TODO: ARREGLAR
    for(int c = getChar(); c != 'c' && c != 'p'; c = getChar()){
        c = getChar();
        if(c == 'c') keepGoing = 0;
        if(c == 'p') keepGoing = 1;
    }
    
    setBuffer(1);
}

void playPong(){
    //p1 left     p2 right
    setBuffer(1);
    Game newGame = myMalloc(sizeof(Game));
    Ball ball = buildBall(10, (1024-30)/2, 768/2, 2, 2);
    Player p1 = buildPlayer(LIMIT_BAR_SPACE), p2 = buildPlayer(SCREEN_WIDTH - LIMIT_BAR_SPACE - 5);

    newGame->ball = ball;
    ball->radius = 10;

    newGame->player1 = p1;
    newGame->player2 = p2;

    newGame->player1->bar->width = LIMIT_BAR_SPACE;
    newGame->player1->bar->height = newGame->player2->bar->height;

    keepGoing = 1;
    while(keepGoing && (p1->score < GAME_OVER || p2->score < GAME_OVER)){
        getInputPlaying(newGame);
        
        updateBall(newGame->ball, newGame->player1, newGame->player2);
        updatePong(newGame);
    }

    myFree(p1->bar);
    myFree(p2->bar);
    myFree(p1);
    myFree(p2);
    myFree(newGame->ball);
    myFree(newGame);

    setBuffer(0);
    terminalSetter();
    return;
}