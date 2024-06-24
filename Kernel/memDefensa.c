
#include <stdint.h>

#define BLOCK_SIZE 512 //tamaño del bloque
#define MEM_START 0xA000
#define MEM_END 0xF000
#define BLOCK_AMOUNT ((MEM_END-MEM_START)/BLOCK_SIZE)
#define NULL ((void *)0)

typedef struct mem_def{
    char used;
    unsigned int start;
    unsigned int end;
} mem_def;


static mem_def memory_array[BLOCK_AMOUNT]; 

void init() {
    for (int i = 0; i < BLOCK_AMOUNT; i++) {
        memory_array[i].used = 0;
    }
}

void * alloc(uint32_t size) {
    if( size <= 0){
        return NULL;
    }

    int blocks_needed = size / BLOCK_SIZE;
    for (int i = 0; i < BLOCK_AMOUNT; i++) {
        if (memory_array[i].used == 0) {
            int j;
            for (j = 0; j < blocks_needed; j++) {
                if (memory_array[i+j].used == 1) {
                    break;
                } 
            }
            if (j == blocks_needed) {
                // marcar block como used=1, i es el start, j el end 
            }
        }
    }
}

void free(void * dir) {

    if (dir == NULL) {
        return;
    }

    int pos = dir_in_arr(dir);
    
    memory_array[pos].used = 0;
    return;
    
}

int dir_in_arr( void* dir ){

    for ( int i=0; i < BLOCK_AMOUNT; i++ ){
        if ( memory_array[i].start == dir && memory_array[i].used){
            return 1;
        }
    }

    return 0;
}