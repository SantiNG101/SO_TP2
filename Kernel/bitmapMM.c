
#ifdef BITMAP

#include <mm.h>


typedef struct MemoryManagerCDT {
    unsigned char bitmap[NUM_BLOCKS];
    unsigned char lengths[NUM_BLOCKS];
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
                    mem_manager->lengths[k] = i; // saves the starting block index of the allocation
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

    int block_index = ((int)((unsigned int *) mem - (unsigned int *) mem_manager->heap_start)) / BLOCK_SIZE;

    if ((block_index % BLOCK_SIZE) != 0)
		return;

    int first_block_index = block_index;
    int blocks_to_free = mem_manager->lengths[block_index];
    
    // if (mem_manager->bitmap[block_index] != BOUNDARY_BLOCK){
	// 	return;
	// }

    while (block_index < NUM_BLOCKS && block_index - first_block_index < blocks_to_free) {
        mem_manager->bitmap[block_index] = FREE;
        block_index++;
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


unsigned int heap_free_bytes() {
    return ( NUM_BLOCKS - get_used_blocks(mem_manager) ) *BLOCK_SIZE;
}

unsigned int heap_used_bytes() {
    return get_used_blocks(mem_manager) * BLOCK_SIZE;
}

#endif

