// TODO: Add a random number generator for fruits.
// TODO: Add a score system.
// TODO: Add a menu.
#include "snake.h"

/**
* @brief  Screen definitions
*/
#define WIDTH           60         // 120 columns of textures
#define HEIGHT          40          // 80 lines of textures
#define REFRESH_RATE    50          // ms

/**
* @brief  Snake definitions
*/
#define INITIAL_SIZE    15           // Snake initial size
#define KEY_UP          'w'
#define KEY_DOWN        's'
#define KEY_LEFT        'a'
#define KEY_RIGHT       'd'

/**
* @brief  Boolean type definition
*/
typedef enum { false = 0, true } boolean;

/**
* @brief  Direction type definition
*/
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
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

    Direction direction;
} Snake;

/**
* @brief Game variables.
*/
static TextureType map[WIDTH][HEIGHT];
static boolean GAME_OVER = false;
static int EXISTING_FRUITS = 0;
static int speed = 1;

void drawOnMap(int x, int y, TextureType textureType){
    map[x][y] = textureType;

    // Draws texture on screen.
    drawTexture(textureType, x, y);
}

/**
* @brief Build a snake with the initial size.
*/
Snake * buildSnake(){
    if(WIDTH < INITIAL_SIZE) return NULL;

    Snake * snake = (Snake *) myMalloc(sizeof(Snake));
    SnakeNode * snakeBody[INITIAL_SIZE];

    // First, we build the snake body.
    // GREATER THE INDEX, GREATER THE X. (RIGHT) SNAKE HEAD IS AT THE RIGHT.
    for(int i = 0; i < INITIAL_SIZE; i++){
        snakeBody[i] = (SnakeNode *) myMalloc(sizeof(SnakeNode));
        snakeBody[i]->x = i + WIDTH/2;
        snakeBody[i]->y = 0 + HEIGHT/2;
        drawOnMap(snakeBody[i]->x, snakeBody[i]->y, i == 0 ? SNAKE_TAIL : i == INITIAL_SIZE - 1 ? SNAKE_HEAD : SNAKE_BODY);
    }

    
    // Then, we link the nodes
    for(int i = 0; i < INITIAL_SIZE - 1; i++) snakeBody[i]->next = snakeBody[i + 1];
    for(int i = INITIAL_SIZE - 1; i > 0; i--) snakeBody[i]->prev = snakeBody[i - 1];
    // Finally, we set the snake head and tail.
    snake->head = snakeBody[INITIAL_SIZE - 1];
    snake->tail = snakeBody[0];
    snake->direction = RIGHT;
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

    if(newHead->x > WIDTH || newHead->x < 0 || newHead->y > HEIGHT || newHead->y < 0){
        GAME_OVER = true;
        return;
    }
    boolean hasEaten = map[newHead->x][newHead->y] == FRUIT ? true : false;

    /**
    * HEAD
    */
    
    drawOnMap(newHead->x, newHead->y, SNAKE_HEAD);
    drawOnMap(snake->head->x, snake->head->y, SNAKE_BODY);

    newHead->prev = snake->head;
    snake->head->next = newHead;
    snake->head = newHead;

    /**
    * TAIL
    */
    if(hasEaten == true) { EXISTING_FRUITS--; speed++;}
    else {
        SnakeNode * newTail = snake->tail->next;
        newTail->prev = NULL;
        drawOnMap(snake->tail->x, snake->tail->y, EMPTY);
        drawOnMap(newTail->x, newTail->y, SNAKE_TAIL);

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
// TODO: Add random position
void addFruit(){
    // FOR NOW...
    if(EXISTING_FRUITS > 0 || map[50][30] != EMPTY) return;
    // TODO: Generate random position
    int x = 50, y = 30;

    if(map[x][y] != EMPTY) return addFruit();
    drawOnMap(x, y, FRUIT);
    EXISTING_FRUITS++;
}



void keyHandler(Snake * snake, int up, int down, int left, int right){
    if(getKeyState(up)) changeDirection(snake, UP);
    else if(getKeyState(down)) changeDirection(snake, DOWN);
    else if(getKeyState(left)) changeDirection(snake, LEFT);
    else if(getKeyState(right)) changeDirection(snake, RIGHT);
}

void playSnake(){
    // setBuffer(1);
    clearScreen(0);
    GAME_OVER = false;
    Snake * snake = buildSnake();


    while(GAME_OVER == false){
        // Sleep for REFRESH_RATE ms, so the game doesn't run too fast.
        sleep(REFRESH_RATE-speed);
        
        addFruit();
        keyHandler(snake, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
        moveSnake(snake);

        updateScreen();
    }

    freeSnake(snake);
    // setBuffer(0);
}