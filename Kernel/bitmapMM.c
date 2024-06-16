
#ifdef BITMAP

#include <mm.h>

typedef struct MemoryManagerCDT {
    unsigned char bitmap[NUM_BLOCKS];
    void * heap_start;
} MemoryManagerCDT;

typedef struct MemoryManagerCDT* MemoryManagerADT;

MemoryManagerADT mem_manager;

void mem_initialize() {
    mem_manager = (MemoryManagerADT) MEM_START;
    mem_manager->heap_start = HEAP_START;

    for (int i = 0; i < NUM_BLOCKS; i++){
        mem_manager->bitmap[i] = FREE;
    }
}

void* memalloc(uint64_t size) {

    if (size == 0) {
        return NULL;
    }

    if (size > MEM_SIZE) {
        return NULL;
    }

    // calculate required blocks
    int blocks_needed = size / BLOCK_SIZE + ((size % BLOCK_SIZE) > 0 ? 1 : 0);

    // search for free blocks
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (mem_manager->bitmap[i] == FREE) {
            int j = i;
            while (j < NUM_BLOCKS && mem_manager->bitmap[j] == FREE && j - i < blocks_needed) {
                j++;
            }
            if (j - i == blocks_needed) {
                for (int k = i; k < j; k++) {
                    mem_manager->bitmap[k] = USED;
                }
                return mem_manager->heap_start + i * BLOCK_SIZE;
            }
        }
    }

    return NULL;
}

void free(void* mem) {

    if (mem == NULL) {
        return;
    }

    if (mem < mem_manager->heap_start || mem >= MEM_STOP) {
        return;
    }

    int block_index = (int)((unsigned int *) mem - (unsigned int *) mem_manager->heap_start);

    if ( ( block_index % BLOCK_SIZE) != 0 )
		return;

	block_index /= BLOCK_SIZE;

    if ( mem_manager->bitmap[block_index] != BOUNDARY_BLOCK){
		return;
	}

    while (block_index < NUM_BLOCKS && mem_manager->bitmap[block_index] == USED)	{
		mem_manager->bitmap[block_index++] = FREE;
	}	

    return;   
}

static MemoryManagerADT get_mem_manager() {
    return mem_manager;
}

// status functions

static unsigned int get_used_blocks() {
    int used = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (mem_manager->bitmap[i] == USED) {
            used++;
        }
    }
	return used;
}

unsigned int get_used_memory(){
	return get_used_blocks(mem_manager) * BLOCK_SIZE;
} 

unsigned long get_total_memory(){
	return MEM_SIZE;
}

unsigned int get_free_memory(){
	return ( NUM_BLOCKS - get_used_blocks(mem_manager) ) *BLOCK_SIZE;
} 

void get_state(char * state){
	int i; 
	for (i = 0; i < NUM_BLOCKS; i++){
		state[i] = mem_manager->bitmap[i]+1;
	}
	state[i] = '\0';
	return;
}

#endif

