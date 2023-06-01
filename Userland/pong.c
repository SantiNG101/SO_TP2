// tamaño de pantalla
// 1024 x 768

typedef struct{
    int x;
    int y;
    int posx;
    int posy;
} Ball;

typedef struct{
    int y;
    int length;
} Bars;

typedef struct
{
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


