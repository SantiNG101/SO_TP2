#include <pong.h>
// tamaño de pantalla
// 1024 x 768
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 1024
#define GAME_OVER 10

typedef struct{
    //posición actual
    int x;
    int y;

    //dirección de movimiento (velocidad) de la pelota
    int posx;
    int posy;

    int radius;
} Ball;

typedef struct{
    int y;
    int length;
} Bars;

typedef struct{
    Bars bar;
    int score;
} Player;

typedef struct{
    Player player1;
    Player player2;
    Ball ball;
} Game;

/* TODO Para la pelota hay que agregar un draw, updatePosition y detector de tocar la barra
    Mismo para las barras */

void resetBall(Ball *ball){
    //reinicio la posición y dirección de la pelota
    ball->x = SCREEN_WIDTH/2;
    ball->y = SCREEN_HEIGHT/2;
    ball->posx = -ball->posx;
    ball->posy = 1;
}

void checkAndHandleWallCollision(Ball *ball, Player *player1, Player *player2){
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
    if(ball.x - ball.radius <= bar.length && ball.x + ball.radius >= 0){
        if(ball.y >= bar.y && ball.y <= bar.y + bar.length){
            return 1; //se detecta colision entre la pelota y la barra
        }
    }
    return 0; //no hay colision
}

void barCollision(Ball *ball, Bars bar){
    //invierto la velocidad en la que se mueve la pelota
    ball->posx = -ball->posx;

    //ajusto la velocidad de la pelota dependiendo sobre que parte de la barra colisiona
    int barCenter = bar.y + bar.length / 2;
    int ballCenter = ball->y;
    int offset = ballCenter - barCenter;
    ball->posy = offset / 2;
}

void updateBar(Bars *bar, int direction){
    //Actualizo posición de la barra
    bar->y += direction;
}

void updateBall(Ball *ball, Player *player1, Player *player2){
    //actualizo la posición de la pelota
    ball->x += ball->posx;
    ball->y += ball->posy;

    //chequeo si hay colisiones con las barras
    if(checkBarCollision(*ball, player1->bar) || checkBarCollision(*ball, player2->bar)){
        barCollision(ball, player1->bar)
    }

    //chequeo si hay colision con los bordes de la pantalla
    checkAndHandleWallCollision(ball, score);

    //chequeo condiciones para fin del juego
    if(player1->score >= GAME_OVER || player2->score >= GAME_OVER){
        // reinicio puntaje y hago un resetBall
        player1->score=0;
        player2->score=0;
        resetBall(ball);
    }
}

void drawBar(Bars bar){
    //implementar funcion que dibuje las barras
}

void drawBall(Ball ball){
    //implementar funcion que dibuje la pelota
}

void drawScore(Game game){
    //implementar funcion que dibuje el puntaje en pantalla
}


