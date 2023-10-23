// TODO: Replace malloc with a custom memory allocator.
// TODO: Add a random number generator for fruits.
// TODO: Add textures.
// TODO: Add a score system.
// TODO: Add a menu.
#include <stdio.h>
#include <time.h>

/**
* @brief  Screen definitions
*/
#define WIDTH           80          // 80 columns of textures
#define HEIGHT          25          // 25 lines of textures
#define REFRESH_RATE    50          // ms

/**
* @brief  Snake definitions
*/
#define INITIAL_SIZE    3           // Snake initial size
#define KEY_UP          0x48
#define KEY_DOWN        0x50
#define KEY_LEFT        0x4B
#define KEY_RIGHT       0x4D

/**
* @brief  Boolean type definition
*/
typedef enum { false, true } bool

/**
* @brief  Texture type definition
*/
typedef enum { 
    EMPTY = 0,
    SNAKE_HEAD,
    SNAKE_BODY,
    SNAKE_TAIL,
    FRUIT,
} TextureType;

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
typedef struct {
    SnakeNode * head;
    SnakeNode * tail;

    Direction direction;
} Snake;

typedef struct {
    int x, y;
    
    SnakeNode * next, * prev;
} SnakeNode;

/**
* @brief Game variables.
*/
static TextureType map[WIDTH][HEIGHT];
static bool GAME_OVER = false;
static int EXISTING_FRUITS = 0;

/**
* @brief Build a snake with the initial size.
*/
Snake * buildSnake(){
    if(WIDTH < INITIAL_SIZE) return NULL;

    Snake * snake = malloc(sizeof(Snake));
    SnakeNode * snakeBody[INITIAL_SIZE];

    // First, we build the snake body.
    for(int i = 0; i < INITIAL_SIZE; i++){
        snakeBody[i] = malloc(sizeof(SnakeNode));
        snakeBody[i]->x = i;
        snakeBody[i]->y = 0;
        snakeBody[i]->next = NULL;
        snakeBody[i]->prev = i != 0 ? snakeBody[i - 1] : NULL;
    }

    // Then, we link the nodes and draw them on the map.
    for(int i = 0; i < INITIAL_SIZE; i++) { 
        snakeBody[i]->next = i != INITIAL_SIZE - 1 ? snakeBody[i + 1] : NULL;
        drawOnMap(snakeBody[i]->x, snakeBody[i]->y, i == 0 ? SNAKE_TAIL : i == INITIAL_SIZE - 1 ? SNAKE_HEAD : SNAKE_BODY);
    }

    // Finally, we set the snake head and tail.
    snake->head = snakeBody[INITIAL_SIZE - 1];
    snake->tail = snakeBody[0];
    return snake;
}

void freeSnake(Snake * snake){
    SnakeNode * current = snake->head;
    while(current != NULL){
        SnakeNode * next = current->next;
        free(current);
        current = next;
    }

    free(snake);
}

/**
* @brief Move the snake one position in the current direction.
*/
void moveSnake(Snake * snake){
    SnakeNode * newHead = malloc(sizeof(SnakeNode));
    newHead->x = snake.head->x;
    newHead->y = snake.head->y;

    if(snake.direction == UP) newHead->y--;
    else if(snake.direction == DOWN) newHead->y++;
    else if(snake.direction == LEFT) newHead->x--;
    else if(snake.direction == RIGHT) newHead->x++;

    if(newHead->x > WIDTH || newHead->x < 0 || newHead->y > HEIGHT || newHead->y < 0 || map[newHead->x][newHead->y] == SNAKE_BODY || map[newHead->x][newHead->y] == SNAKE_HEAD) 
    return GAME_OVER = true;
    bool hasEaten = map[newHead->x][newHead->y] == FRUIT;

    /**
    * HEAD
    */
    newHead->next = snake.head;
    snake.head->prev = newHead;
    drawOnMap(newHead->x, newHead->y, SNAKE_HEAD);
    drawOnMap(snake.head->x, snake.head->y, SNAKE_BODY);

    snake.head = newHead;

    /**
    * TAIL
    */
    if(hasEaten) EXISTING_FRUITS--;
    else {
        SnakeNode * newTail = snake.tail->prev;
        newTail->next = NULL;
        drawOnMap(newTail->x, newTail->y, SNAKE_TAIL);
        drawOnMap(snake.tail->x, snake.tail->y, EMPTY);

        // Free old tail
        free(snake.tail);
        snake.tail = newTail;
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
    if(EXISTING_FRUITS > 0) return;
    // TODO: Generate random position
    int x, y;

    if(map[x][y] != EMPTY) return addFruit();
    drawOnMap(x, y, FRUIT);
    EXISTING_FRUITS++;
}

void drawOnMap(int x, int y, TextureType textureType){
    map[x][y] = texture;

    // TODO: Add textures.
    // Obtains texture from texture type.
    Texture texture = getTexture(textureType);

    // Draws texture on screen.
    int screenX = x * TEXTURE_WIDTH;
    int screenY = y * TEXTURE_HEIGHT;

    for(int i = 0; i < TEXTURE_WIDTH; i++)
        for(int j = 0; j < TEXTURE_HEIGHT; j++)
            putPixel(screenX + i, screenY + j, texture[i][j]);
}

void keyHandler(Snake * snake, int up, int down, int left, int right){
    if(getKeyState(up)) changeDirection(snake, UP);
    else if(getKeyState(down)) changeDirection(snake, DOWN);
    else if(getKeyState(left)) changeDirection(snake, LEFT);
    else if(getKeyState(right)) changeDirection(snake, RIGHT);
}

void playSnake(){
    Snake * snake = buildSnake();

    while(!GAME_OVER){
        moveSnake(snake);

        addFruit();
        keyHandler(snake, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);

        // Sleep for REFRESH_RATE ms, so the game doesn't run too fast.
        sleep(REFRESH_RATE); 
    }

    freeSnake(snake);
}