
// TODO: Add a menu.
#include "snake.h"
#include "songs.h"

/**
* @brief  Screen definitions
*/
#define WIDTH           60         // 120 columns of textures
#define HEIGHT          40          // 80 lines of textures
#define REFRESH_RATE    70          // ms

/**
* @brief  Snake definitions
*/
#define INITIAL_SIZE    3           // Snake initial size
#define KEY_UP          'w'
#define KEY_DOWN        's'
#define KEY_LEFT        'a'
#define KEY_RIGHT       'd'

#define KEY_UP2         'i'
#define KEY_DOWN2       'k'
#define KEY_LEFT2       'j'
#define KEY_RIGHT2      'l'

/**
* @brief  Boolean type definition
*/
typedef enum { false = 0, true } boolean;

/**
* @brief  Direction type definition
*/
typedef enum {
    RIGHT,
    DOWN,
    UP,
    LEFT
} Direction;

/**
* @brief  Snake type definition
*/
typedef struct snakeNode {
    int x, y;

    struct snakeNode * prev, * next;
} SnakeNode;

typedef struct {
    SnakeNode * head;
    SnakeNode * tail;
    int score, snakeNumber;
    uint64_t colour;

    Direction direction;
} Snake;

/**
* @brief Game variables.
*/
static TextureType map[WIDTH][HEIGHT];
static int GAME_OVER = 0;
static int EXISTING_FRUITS = 0;
static int speed = 1;

void drawOnMap(int x, int y, TextureType textureType, uint64_t colour){
    map[x][y] = textureType;

    // Draws texture on screen.
    drawTexture(textureType, x, y, colour);
}


/**
* @brief Build a snake with the initial size.
*/
Snake * buildSnake(int offsetX, int offsetY, int snakeNumber, uint64_t colour){
    if(WIDTH < INITIAL_SIZE) return NULL;

    Snake * snake = (Snake *) myMalloc(sizeof(Snake));
    SnakeNode * snakeBody[INITIAL_SIZE];

    // First, we build the snake body.
    // GREATER THE INDEX, GREATER THE X. (RIGHT) SNAKE HEAD IS AT THE RIGHT.
    for(int i = 0; i < INITIAL_SIZE; i++){
        snakeBody[i] = (SnakeNode *) myMalloc(sizeof(SnakeNode));
        snakeBody[i]->x = i + WIDTH/2 + offsetX;
        snakeBody[i]->y = 0 + HEIGHT/2 + offsetY;
        drawOnMap(snakeBody[i]->x, snakeBody[i]->y, i == 0 ? SNAKE_TAIL : i == INITIAL_SIZE - 1 ? SNAKE_HEAD : SNAKE_BODY, colour);
    }


    // Then, we link the nodes
    for(int i = 0; i < INITIAL_SIZE - 1; i++) snakeBody[i]->next = snakeBody[i + 1];
    for(int i = INITIAL_SIZE - 1; i > 0; i--) snakeBody[i]->prev = snakeBody[i - 1];
    // Finally, we set the snake head and tail.
    snake->head = snakeBody[INITIAL_SIZE - 1];
    snake->tail = snakeBody[0];
    snake->score = 0;
    snake->direction = RIGHT;
    snake->snakeNumber = snakeNumber;
    snake->colour = colour;
    return snake;
}

void freeSnake(Snake * snake){
    SnakeNode * current = snake->head;
    while(current != NULL){
        SnakeNode * prev = current->prev;
        myFree(current);
        current = prev;
    }

    myFree(snake);
}

/**
* @brief Move the snake one position in the current direction.
*/
void moveSnake(Snake * snake){
    SnakeNode * newHead = (SnakeNode *) myMalloc(sizeof(SnakeNode));
    newHead->x = snake->head->x;
    newHead->y = snake->head->y;

    if(snake->direction == UP) newHead->y--;
    else if(snake->direction == DOWN) newHead->y++;
    else if(snake->direction == LEFT) newHead->x--;
    else if(snake->direction == RIGHT) newHead->x++;

    if(newHead->x > WIDTH || newHead->x < 0 || newHead->y > HEIGHT || newHead->y < 0 || (map[newHead->x][newHead->y] != EMPTY && map[newHead->x][newHead->y] != FRUIT)){
        GAME_OVER = snake->snakeNumber;
        return;
    }

    boolean hasEaten = map[newHead->x][newHead->y] == FRUIT ? true : false;

    /**
    * HEAD
    */

    drawOnMap(newHead->x, newHead->y, SNAKE_HEAD+snake->direction, snake->colour);

    drawOnMap(snake->head->x, snake->head->y, SNAKE_BODY, snake->colour);

    newHead->prev = snake->head;
    snake->head->next = newHead;
    snake->head = newHead;

    /**
    * TAIL
    */
    if(hasEaten == true) { 
        EXISTING_FRUITS--; 
        speed++; 
        snake->score++;
        startSound(440);
    }
    else {
        SnakeNode * newTail = snake->tail->next;
        newTail->prev = NULL;
        drawOnMap(snake->tail->x, snake->tail->y, EMPTY, BLACK);
        int x = snake->tail->x - newTail->x;
        int y = snake->tail->y - newTail->y;

        // | x  | y  | texture
        // | 0  | 1  | SNAKE_TAIL (BODY AT TOP)
        // | 0  | -1 | SNAKE_TAIL (BODY AT BOTTOM)
        // | 1  | 0  | SNAKE_TAIL (BODY AT LEFT)
        // | -1 | 0  | SNAKE_TAIL (BODY AT RIGHT)

        // NOT POSSIBLE, Because the snake can't go back.
        // | 1  | 1  |
        // | 1  | -1 |
        // | -1 | 1  |
        // | -1 | -1 |
        if(x == 0 && y == 1) drawOnMap(newTail->x, newTail->y, SNAKE_TAIL+UP, snake->colour);
        else if(x == 0 && y == -1) drawOnMap(newTail->x, newTail->y, SNAKE_TAIL+DOWN, snake->colour);
        else if(x == 1 && y == 0) drawOnMap(newTail->x, newTail->y, SNAKE_TAIL+LEFT, snake->colour);
        else if(x == -1 && y == 0) drawOnMap(newTail->x, newTail->y, SNAKE_TAIL+RIGHT, snake->colour);

        // Free old tail
        myFree(snake->tail);
        snake->tail = newTail;
    }


}

void changeDirection(Snake * snake, Direction direction){
    if(snake->direction == UP && direction == DOWN) return;
    if(snake->direction == DOWN && direction == UP) return;
    if(snake->direction == LEFT && direction == RIGHT) return;
    if(snake->direction == RIGHT && direction == LEFT) return;

    snake->direction = direction;
}

/**
* @brief Add a fruit to the map if necessary.
*/
void addFruit(){
    if(EXISTING_FRUITS > 0) return;
    unsigned int x = mi_rand() % WIDTH, y = mi_rand() % HEIGHT;

    if(map[x][y] != EMPTY) return addFruit();
    drawOnMap(x, y, FRUIT, RED);
    EXISTING_FRUITS++;
}



void keyHandler(Snake * snake, int up, int down, int left, int right){
    if(getKeyState(up)) changeDirection(snake, UP);
    else if(getKeyState(down)) changeDirection(snake, DOWN);
    else if(getKeyState(left)) changeDirection(snake, LEFT);
    else if(getKeyState(right)) changeDirection(snake, RIGHT);
}

void printMenuSnake(){
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
        printf("SNAKE");
        setPrintAnywhere(360 + NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Press 1 to play\n");
        setPrintAnywhere(360+2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Press 2 to play 1v1\n");
        setPrintAnywhere(360+3*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Press 3 to exit");
}

/**
* @brief  Play snake.
*/
void playSnake(char option){
    for(int i = 0; i < WIDTH; i++) 
        for(int j = 0; j < HEIGHT; j++)
            map[i][j] = EMPTY;

    GAME_OVER = 0;
    EXISTING_FRUITS = 0;
    speed = 1;
    
    clearScreen(0);

    Snake * snake = buildSnake(-5,-5, 1, GREEN);
    Snake * snake2;

    if(option == '2') {
        snake2 = buildSnake(5,5, 2, CYAN);
        snake2->direction = DOWN;
    }

    while(GAME_OVER == 0){
        // Sleep for REFRESH_RATE ms, so the game doesn't run too fast.
        sleep(REFRESH_RATE-speed);
        stopSound();

        addFruit();
        keyHandler(snake, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
        moveSnake(snake);
        draw_number(MIDDLE_SCREEN-MARQUER_DISTANCE_X+3*NUMBER_WIDTH,MARQUER_DISTANCE_Y,snake->score,COLOR_LETTER_DEFAULT,COLOR_BACKGROUND_DEFAULT);

        if(option == '2'){
            keyHandler(snake2, KEY_UP2, KEY_DOWN2, KEY_LEFT2, KEY_RIGHT2);
            moveSnake(snake2);
            draw_number(MIDDLE_SCREEN-MARQUER_DISTANCE_X-3*NUMBER_WIDTH,MARQUER_DISTANCE_Y,snake2->score,COLOR_LETTER_DEFAULT,COLOR_BACKGROUND_DEFAULT);
        }

        updateScreen();
    }

    clearScreen(0);
    setPrintAnywhere(360,MIDDLE_SCREEN-30);
    printf("GAME OVER");
    if(option == '2'){
        setPrintAnywhere(360 + NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Player %d won!", (GAME_OVER%2)+1);

        setPrintAnywhere(360 + 2*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Player 1 score: %d", snake->score);
        setPrintAnywhere(360 + 3*NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Player 2 score: %d", snake2->score);
    }
    else{
        setPrintAnywhere(360 + NUMBER_HEIGHT,MIDDLE_SCREEN-30);
        printf("Your score: %d", snake->score);
    }
    
    
    playSound(330, 100); // Nota E
    playSound(294, 150); // Nota D#
    playSound(262, 100); // Nota C#
    playSound(220, 150); // Nota A
    playSound(196, 150); // Nota G
    playSound(175, 100); // Nota F
    playSound(165, 150); // Nota E
    playSound(147, 100); // Nota D
    playSound(330, 100); // Nota E
    playSound(196, 150); // Nota G

    sleep(3000);

    freeSnake(snake);
}


/**
*  
*/
void menuSnake(){
    clearScreen(0);
    printMenuSnake();
    char option;

    while((option = getChar())){
        if(option == '1' || option == '2') playSnake(option);
        if(option == '3') return;

        printMenuSnake();
    }
}