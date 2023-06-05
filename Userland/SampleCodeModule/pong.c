#include "pong.h"
#include <stdlib.h>
// tamaño de pantalla
// 1024 x 768
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define GAME_OVER 3
#define BAR_MOV 10
#define DEFAULT_BRADIUS 10
#define DEFAULT_BARSPEED 10
#define DEFAULT_BALLSPEED 8
extern void setPrintAnywhere(uint32_t y, uint32_t x);

static int keepGoing = 1;

extern void terminalSetter();

// punto x e y de la pelota hacen referencia al centro de la pelota
typedef struct Ball {
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
    int pace;
} * Bars;


typedef struct Player {
    Bars bar;
    int score;
} * Player;


typedef struct Game {
    Player player1;
    Player player2;
    Ball ball;
} * Game;

static void updatePong(Game newGame){
    clearScreen(0);
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
        ballBeep();
        player1->score++;
        resetBall(ball);
        return;
    }

    if(ball->x - ball->radius <= 0){
        //player2 suma un punto y se reinicia la posición y velocidad
        ballBeep();
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
    bar->y += direction*(bar->pace);
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

void getInputPlaying(Game game){
    if(game->player2->bar->y + game->player2->bar->height < SCREEN_HEIGHT && game->player2->bar->y > 0) {
        if (getKeyState(0x80)) updateBar(game->player2->bar, -1);
        if (getKeyState(0x81)) updateBar(game->player2->bar, 1);
    }
    if(game->player1->bar->y + game->player1->bar->height < SCREEN_HEIGHT && game->player1->bar->y > 0) {
        if (getKeyState('w'))updateBar(game->player1->bar, -1);
        if (getKeyState('s'))updateBar(game->player1->bar, 1);
    }
    if(game->player1->bar->y <= 0) {
        if (getKeyState('s'))updateBar(game->player1->bar, 1);
    }
    if(game->player2->bar->y <= 0){
        if (getKeyState(0x81)) updateBar(game->player2->bar, 1);
    }
    if(game->player1->bar->y + game->player1->bar->height >= SCREEN_HEIGHT) {
        if (getKeyState('w'))updateBar(game->player1->bar, -1);
    }
    if(game->player2->bar->y + game->player2->bar->height >= SCREEN_HEIGHT){
        if (getKeyState(0x80)) updateBar(game->player2->bar, -1);
    }

    if(getKeyState(0x1B))pausePong();

    return;
}

Player buildPlayer(int barX, int pace){
    Player player = myMalloc(sizeof(struct Player));
    Bars bar = player->bar = myMalloc(sizeof(struct Bars));

    player->score = 0;

    bar->width = 30;
    bar->height = 120;
    bar-> y = (768/2) - 60;
    bar->x = barX;
    bar->pace = pace;

    return player;
}

Ball buildBall(int radius, int x, int y, int posx, int posy){
    Ball ball = myMalloc(sizeof(struct Ball));

    ball->x = x;
    ball->y = y;
    ball->posx = posx;
    ball->posy = posy;
    ball->radius = radius;

    return ball;
}

void pausePong(){
    // Imprimo el menú
    setBuffer(0);

    printf("PAUSA: \n");
    printf("Para continuar presione [ P ]\n");
    printf("Para cerrar el juego presione [ C ]");

    for(int c = 0; c != 'c' && c != 'p'; ){
        c = getChar();
        if(c == 'c') keepGoing = 0;
        if(c == 'p') keepGoing = 1;
    }


    setBuffer(1);
}


void playPong(int ballRadius, int ballSpeed, int barSpeed){
    //p1 left     p2 right
    setBuffer(1);

    Game newGame = myMalloc(sizeof(struct Game));
    Ball ball = buildBall(ballRadius, (1024-30)/2, 768/2, ballSpeed, ballSpeed);
    Player p1 = buildPlayer(LIMIT_BAR_SPACE,barSpeed);
    Player p2 = buildPlayer(SCREEN_WIDTH - LIMIT_BAR_SPACE - 35,barSpeed);

    newGame->ball = ball;

    newGame->player1 = p1;
    newGame->player2 = p2;

    keepGoing = 1;
    while(keepGoing && (p1->score < GAME_OVER && p2->score < GAME_OVER)){
        getInputPlaying(newGame);

        updateBall(newGame->ball, newGame->player1, newGame->player2);
        updatePong(newGame);
    }

    setBuffer(0);
    clearScreen(0);
    setPrintAnywhere(383,MIDDLE_SCREEN-30);
    if(p1->score == 3){
        printf("GANADOR JUGADOR 1\n");
        sleep(5000);
    } else if (p2->score ==3) {
        printf("GANADOR JUGADOR 2\n");
        sleep(5000);
    }



    myFree(p1->bar);
    myFree(p2->bar);
    myFree(p1);
    myFree(p2);
    myFree(newGame->ball);
    myFree(newGame);


    terminalSetter();
    return;
}

void printSettings(){
    clearScreen(0);
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
    printf("Press 1 to modify ballradius\n");
    setPrintAnywhere(360+NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 2 to modify ballspeed\n");
    setPrintAnywhere(360+2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 3 to modify barspeed\n");
    setPrintAnywhere(360+3*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 4 to return to menu.");
    return;
}

void settings(int * bRad, int * bSpeed, int * barSpeed){
    printSettings();

    char c;
    while ((c = getChar())) {
        switch (c) {
            case '1':
                clearScreen(0);
                setPrintAnywhere(360,MIDDLE_SCREEN-30);
                printf("Inserte nuevo radio: ");
                scanf("%d",bRad);
                printSettings();
                break;
            case '2':
                clearScreen(0);
                setPrintAnywhere(360,MIDDLE_SCREEN-30);
                printf("Inserte nueva velocidad: ");
                scanf("%d",bSpeed);
                printSettings();
                break;
            case '3':
                clearScreen(0);
                setPrintAnywhere(360,MIDDLE_SCREEN-30);
                printf("Inserte nuevo velocidad: ");
                scanf("%d",barSpeed);
                printSettings();
                break;
            case '4':
                clearScreen(0);
                setPrintAnywhere(360,MIDDLE_SCREEN-30);
                return;
            default:
                break;
        }
    }
}

void printMenu(){
    terminalSetter();
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
    printf("Press 1 to play\n");
    setPrintAnywhere(360+NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 2 for Settings\n");
    setPrintAnywhere(360+2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 3 to exit");
}

void menuPong(){
    int * ballRadius = myMalloc(sizeof(int));
    int * ballSpeed = myMalloc(sizeof(int));
    int * barSpeed = myMalloc(sizeof(int));

    *ballRadius = DEFAULT_BRADIUS;
    *ballSpeed = DEFAULT_BALLSPEED;
    *barSpeed = DEFAULT_BARSPEED;

    printMenu();

    char c;
    while ((c = getChar())) {
        switch (c) {
            case '1':
                playPong(*ballRadius,*ballSpeed,*barSpeed);
                printMenu();
                break;
            case '2':
                settings(ballRadius,ballSpeed,barSpeed);
                printMenu();
                break;
            case '3':
                return;
            default:
                break;
        }
    }

    myFree(ballRadius);
    myFree(ballSpeed);
    myFree(barSpeed);
}