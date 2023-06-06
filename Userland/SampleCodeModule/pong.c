#include "pong.h"
#include <stdlib.h>
// tamaño de pantalla
// 1024 x 768
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

//Struct de colores para el pong
typedef struct{
    char color[15];
    int hex;
}colour2;
//Array del struct para mapear los colores con su valor hexa
const colour2 colors2[]= {{"black",BLACK},{"blue",BLUE},{"green",GREEN},
                        {"cyan",CYAN},{"red",RED},{"purple",PURPLE},
                        {"brown",BROWN},{"gray",GRAY},{"darkgray",DARK_GRAY},
                        {"lightblue",LIGHT_BLUE},{"lime",LIGHT_GREEN},{"lightcyan",LIGHT_CYAN},
                        {"lightred",LIGHT_RED},{"lightpurple",LIGHT_PURPLE},{"yellow",YELLOW},
                        {"white",WHITE}};

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

    uint64_t hexColor;
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
    uint64_t colorHex;
} * Player;

typedef struct Game {
    Player player1;
    Player player2;
    Ball ball;
} * Game;

/*Esta funcion dibuja las figuras del pong en las posiciones actualizadas llama a la syscall updateScreen*/
static void updatePong(Game newGame){
    clearScreen(0);
    draw_Rectangle(newGame->player1->bar->x, newGame->player1->bar->y, newGame->player1->bar->width, newGame->player1->bar->height, newGame->player1->colorHex);
    draw_Rectangle(newGame->player2->bar->x, newGame->player2->bar->y, newGame->player2->bar->width, newGame->player2->bar->height,newGame->player2->colorHex);
    draw_Line (MIDDLE_SCREEN-1,0,MIDDLE_SCREEN-1,768,newGame->player1->colorHex);
    draw_Line (MIDDLE_SCREEN-2,0,MIDDLE_SCREEN-2,768,newGame->player1->colorHex);
    draw_Line (MIDDLE_SCREEN+2,0,MIDDLE_SCREEN+2,768,newGame->player2->colorHex);
    draw_Line (MIDDLE_SCREEN+3,0,MIDDLE_SCREEN+3,768,newGame->player2->colorHex);
    draw_number(MIDDLE_SCREEN-MARQUER_DISTANCE_X-3*NUMBER_WIDTH,MARQUER_DISTANCE_Y,newGame->player1->score,newGame->player1->colorHex,COLOR_BACKGROUND_DEFAULT);
    draw_number(MIDDLE_SCREEN+MARQUER_DISTANCE_X+3*NUMBER_WIDTH,MARQUER_DISTANCE_Y,newGame->player2->score,newGame->player2->colorHex,COLOR_BACKGROUND_DEFAULT);
    draw_CircleFilled(newGame->ball->x, newGame->ball->y,newGame->ball->radius, newGame->ball->hexColor);

    updateScreen();
}

/*Devuelve la pelota a la posicion inicial en el medio de la cancha del pong*/
void resetBall(Ball ball){
    //reinicio la posición y dirección de la pelota
    ball->x = (SCREEN_WIDTH-30)/2;
    ball->y = SCREEN_HEIGHT/2;
    ball->posx = -ball->posx;
    ball->posy = 2;
}

/*Revisa colisiones con los bordes de la cancha del pong:
 * Pared izquierda / derecha genera un gol para el respectivo jugador.
 * Pared superior e inferior generan un rebote.
 * */
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

/*Auxiliar para revisar la colision de la pelota con la barra*/
int isOnRange(int left, int right, int value){
    return value >= left && value <= right;
}

/*revisa si hay colision con la barra*/
int checkBarCollision(Ball ball, Bars bar){
    int leftCollision = isOnRange(bar->x, bar->x + bar->width, ball->x + ball->radius);
    int rightCollision = isOnRange(bar->x, bar->x + bar->width, ball->x - ball->radius);
    int yCollision = isOnRange(bar->y, bar->y + bar->height, ball->y);

    return (leftCollision || rightCollision) && yCollision;
}

//Handler de la colision con la barra
void barCollision(Ball ball, Bars bar){
    //invierto la velocidad en la que se mueve la pelota
    ball->posx = -ball->posx;

    //ajusto la velocidad de la pelota dependiendo sobre que parte de la barra colisiona
    int barCenter = bar->y + (bar->height / 2);
    int ballCenter = ball->y;
    int offset = ballCenter - barCenter;
    ball->posy = offset / 2;
}

//Translacion de la barra verticalmente
void updateBar(Bars bar, int direction){
    //Actualizo posición de la barra
    bar->y += direction*(bar->pace);
}

//Movimiento de la pelota de un punto en la pantalla a otro
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

//Handler del teclado para el pong
void getInputPlaying(Game game){
    //0x80 flecha abajo 0x81 flecha arriba
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

//Inicializa al objeto jugador
Player buildPlayer(int barX, int pace, uint64_t cHex){
    Player player = myMalloc(sizeof(struct Player));
    Bars barr = myMalloc(sizeof(struct Bars));

    player->score = 0;

    barr->width = 30;
    barr->height = 120;
    barr-> y = (768/2) - 60;
    barr->x = barX;
    barr->pace = pace;

    player->bar = barr;
    player->colorHex = cHex;
    return player;
}

//Inicializa al objeto bola
Ball buildBall(int radius, int x, int y, int posx, int posy, uint64_t cHex){
    Ball ball = myMalloc(sizeof(struct Ball));

    ball->x = x;
    ball->y = y;
    ball->posx = posx;
    ball->posy = posy;
    ball->radius = radius;
    ball->hexColor = cHex;
    return ball;
}

//Handler para la tecla ESC
void pausePong(){
    // Imprimo el menú
    setBuffer(0);

    printf("PAUSE: \n");
    printf("To continue press [ P ]\n");
    printf("For end this game press [ C ]");

    for(int c = 0; c != 'c' && c != 'p'; ){
        c = getChar();
        if(c == 'c') keepGoing = 0;
        if(c == 'p') keepGoing = 1;
    }


    setBuffer(1);
}

//Inicializa el juego pong con todos los objetos
void playPong(int ballRadius, int ballSpeed, int barSpeed, uint64_t ballColor, uint64_t p1Color, uint64_t p2Color, int topScore){
    //p1 left     p2 right
    setBuffer(1);

    Game newGame = myMalloc(sizeof(struct Game));
    Ball ball = buildBall(ballRadius, (1024-30)/2, 768/2, ballSpeed, ballSpeed,ballColor);
    Player p1 = buildPlayer(LIMIT_BAR_SPACE,barSpeed,p1Color);
    Player p2 = buildPlayer(SCREEN_WIDTH - LIMIT_BAR_SPACE - 35,barSpeed,p2Color);

    newGame->ball = ball;

    newGame->player1 = p1;
    newGame->player2 = p2;

    keepGoing = 1;
    while(keepGoing && (p1->score < topScore && p2->score < topScore)){
        getInputPlaying(newGame);

        updateBall(newGame->ball, newGame->player1, newGame->player2);
        updatePong(newGame);
    }

    setBuffer(0);
    clearScreen(0);
    setPrintAnywhere(383,MIDDLE_SCREEN-30);
    if(p1->score == topScore){
        printf("GANADOR JUGADOR 1\n");
        sleep(5000);
    } else if (p2->score == topScore) {
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

//Imprime la configuracion del pong
void printSettings(){
    clearScreen(0);
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
    printf("PONG");
    setPrintAnywhere(360+NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 1 to modify ballradius\n");
    setPrintAnywhere(360+2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 2 to modify ballspeed\n");
    setPrintAnywhere(360+3*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 3 to modify barspeed\n");
    setPrintAnywhere(360+4*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 4 to modify ballColor\n");
    setPrintAnywhere(360+5*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 5 to modify p1Color\n");
    setPrintAnywhere(360+6*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 6 to modify p2Color\n");
    setPrintAnywhere(360+7*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 7 to return to change end score\n");
    setPrintAnywhere(360+8*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 8 to return to menu.");
    return;
}

//Funcion auxiliar para elegir un color en settings
void getColor(uint64_t * color){

    char * aux = myMalloc(sizeof(char)*20);

    int done = 0;
    while(!done) {
        clearScreen(0);
        setPrintAnywhere(360,MIDDLE_SCREEN-300);
        for(int j = 0; j < SIZEOFARR(colors2)-1; j++){
            printf("%s, ",colors2[j].color);
        }

        setPrintAnywhere(360+NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Please choose from the previous list: ");
        scanf("%s",aux);

        for (int i = 0; i < SIZEOFARR(colors2); i++) {
            if (!strcmp(colors2[i].color, aux)) {
                *color = colors2[i].hex;
                myFree(aux);
                return;
            }
        }
    }
}

//Maneja las opciones en el pong, entre ellas:
/*
 * ballSpeed -> Velocidad de la pelota
 * ballRadius -> radio de la pelota
 * barSpeed -> velocidad de movimiento de la barra
 * ballColor -> color de la pelota
 * p1Color -> idem para el jugador1
 * p2Color -> idem para el jugador2
 * topScore -> Puntaje necesario para ganar
 */
void settings(int * bRad, int * bSpeed, int * barSpeed,uint64_t * ballColor, uint64_t * p1Color, uint64_t * p2Color, int * topScore){
    printSettings();

    char c;
    while ((c = getChar())) {
        switch (c) {
            case '1':
                *bRad = -1;
                while(*bRad < 1 || *bRad > 50){
                    clearScreen(0);
                    setPrintAnywhere(360,MIDDLE_SCREEN-30);
                    printf("Inserte nuevo radio entre 1 y 50: ");
                    scanf("%d",bRad);
                }
                printSettings();
                break;
            case '2':
                *bSpeed = -1;
                while(*bSpeed < 1 || *bSpeed > 30){
                    clearScreen(0);
                    setPrintAnywhere(360,MIDDLE_SCREEN-30);
                    printf("Inserte nueva velocidad entre 1 y 30: ");
                    scanf("%d",bSpeed);
                }
                printSettings();
                break;
            case '3':
                *barSpeed = -1;
                while(*barSpeed < 1 || *barSpeed > 30){
                    clearScreen(0);
                    setPrintAnywhere(360,MIDDLE_SCREEN-30);
                    printf("Inserte nueva velocidad entre 1 y 30: ");
                    scanf("%d",barSpeed);
                }
                printSettings();
                break;
            case '4':
                getColor(ballColor);
                printSettings();
                break;
            case '5':
                getColor(p1Color);
                printSettings();
                break;
            case '6':
                getColor(p2Color);
                printSettings();
                break;
            case '7':
                *topScore = -1;
                while(*topScore < 1){
                    clearScreen(0);
                    setPrintAnywhere(360,MIDDLE_SCREEN-30);
                    printf("Inserte maxima cantidad de puntos: ");
                    scanf("%d",topScore);
                }
                printSettings();
                break;
            case '8':
                clearScreen(0);
                setPrintAnywhere(360,MIDDLE_SCREEN-30);
                return;
            default:
                break;
        }
    }
}

//Imprime el menu inicial del Pong
void printMenu(){
    clearScreen(0);
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
    printf("PONG");
    setPrintAnywhere(360 + NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 1 to play\n");
    setPrintAnywhere(360+2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 2 for Settings\n");
    setPrintAnywhere(360+3*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
    printf("Press 3 to exit");
}

//Inicializa el pong y reserva espacio para las variables que se pueden manipular desde settings.
void menuPong(){
    int * ballRadius = myMalloc(sizeof(int)*2);
    int * ballSpeed = myMalloc(sizeof(int)*2);
    int * barSpeed = myMalloc(sizeof(int)*2);
    uint64_t * ballColor = myMalloc(sizeof(uint64_t)*2);
    uint64_t * p1Color = myMalloc(sizeof(uint64_t)*2);
    uint64_t * p2Color = myMalloc(sizeof(uint64_t)*2);
    int * topScore = myMalloc(sizeof(int)*2);

    *ballRadius = DEFAULT_BRADIUS;
    *ballSpeed = DEFAULT_BALLSPEED;
    *barSpeed = DEFAULT_BARSPEED;
    *ballColor = DEFAULT_BC;
    *p1Color = DEFAULT_P1C;
    *p2Color = DEFAULT_P2C;
    *topScore = GAME_OVER;

    printMenu();

    char c;
    while ((c = getChar())) {
        switch (c) {
            case '1':
                playPong(*ballRadius,*ballSpeed,*barSpeed,*ballColor,*p1Color,*p2Color,*topScore);
                printMenu();
                break;
            case '2':
                settings(ballRadius,ballSpeed,barSpeed,ballColor,p1Color,p2Color,topScore);
                printMenu();
                break;
            case '3':
                myFree(ballRadius);
                myFree(ballSpeed);
                myFree(barSpeed);
                myFree(ballColor);
                myFree(p1Color);
                myFree(p2Color);
                myFree(topScore);
                return;
            default:
                break;
        }
    }
}