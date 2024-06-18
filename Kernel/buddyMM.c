#ifdef BUDDY
#include <mm.h>

#define BYTE_ALIGMENT 8
#define MASK_BYTE_ALIGMENT 0x07

typedef struct BlockCDT {
    unsigned char status;
    int history;
    struct BlockCDT * next;
    unsigned int block_size;
} BlockCDT;

typedef struct BlockCDT * BlockADT;

typedef struct MemoryManagerCDT {
    BlockADT start;
    BlockADT end;
    unsigned int free_bytes;
} MemoryManagerCDT;

typedef struct MemoryManagerCDT * MemoryManagerADT;



static const uint16_t STRUCT_SIZE = ((sizeof(BlockCDT) + (BYTE_ALIGMENT - 1)) & ~MASK_BYTE_ALIGMENT);
static void remove_block(BlockADT block);
static int add_block(BlockADT block, unsigned char merge);

MemoryManagerADT mem_manager = (MemoryManagerADT) MEM_START;

void mem_initialize() {
    mem_manager->free_bytes = HEAP_SIZE;

    BlockADT first_block = (BlockADT) HEAP_START;

    mem_manager->start->next = (void *) first_block;
    mem_manager->start->block_size = 0;

    mem_manager->end->next = NULL;
    mem_manager->end->block_size = HEAP_SIZE;

    first_block->block_size = HEAP_SIZE;
    first_block->next = &mem_manager->end;

    first_block->status = FREE;
    first_block->history = 0;
}

void* memalloc(uint64_t size) {

    if (size == 0) {
        return NULL;
    }

    void * allocated_mem = NULL;

    size += STRUCT_SIZE;

    if ((size & MASK_BYTE_ALIGMENT) != 0) {
        size += (BYTE_ALIGMENT - (size & MASK_BYTE_ALIGMENT));
    }

    if (size > HEAP_SIZE) {
        return NULL;
    }

    BlockADT current = &mem_manager->start;
    BlockADT previous = mem_manager->start->next;

    while ((current->block_size < size) && (current->next != NULL)) {
        previous = current;
        current = current->next;
    }

    if (current == &mem_manager->end) {
        return NULL;
    }

    allocated_mem = (void *) (((unsigned int) previous->next) + STRUCT_SIZE);

    previous->next = current->next;

    while (current->block_size / 2 >= MIN_BLOCK_SIZE  && current->block_size / 2 >= size) {
        current->block_size /= 2;
        current->history = current->history << 1;
        
        BlockADT new = (void *)((unsigned int) current + current->block_size);
        new->block_size = current->block_size;
        new->status = FREE;
        new->history = current->history | 0x1;
        add_block(new, 0);
    }
    mem_manager->free_bytes -= current->block_size;
    current->status = USED;

    return allocated_mem;
}

void free(void* mem) {
    if (mem == NULL) {
        return;
    }   

    int * mem_free = (int *) mem;
    mem_free -= STRUCT_SIZE;
    BlockADT block_free = (void *) mem_free;
    unsigned int block_size = block_free->block_size;

    add_block((BlockADT) block_free, 1);
    mem_manager->free_bytes += block_size;
}

static void remove_block(BlockADT block) {
    BlockADT aux = &mem_manager->start;

    while (aux != NULL && aux->next != block) {
        aux = aux->next;
    }

    if (aux != NULL) {
        aux->next = aux->next->next;
    }
}

static int add_block(BlockADT block, unsigned char merge) {
    if (block->block_size == HEAP_SIZE) {
        mem_manager->start->next = block;
        block->next = &mem_manager->end;
        block->status = FREE;
        return block->block_size;
    }
    BlockADT buddy = NULL;
    int aux_merge = 0;

    if (merge) {
        if ((block->history & 0x1) == 1) {
            // right block
            buddy = (BlockADT) ((unsigned int) block - block->block_size);
            if (buddy->status == FREE && buddy->block_size == block->block_size) {
                buddy->block_size *= 2;
                aux_merge = 1;
                block = buddy;
            }
        } else {
            buddy = (BlockADT) ((unsigned int) block + block->block_size);
            if (buddy->status == FREE && buddy->block_size == block->block_size) {
                buddy->block_size *= 2;
                aux_merge = 1;
            }
        }
    }

    if (aux_merge == 1) {
        block->history = block->history >> 1;
        remove_block(buddy);
        return add_block(block, 1);
    }

    BlockADT aux = &mem_manager->start;
    int block_size = block->block_size;

    while (aux->next->block_size < block_size) {
        aux = aux->next;
    }

    block->next = aux->next;
    aux->next = block;
    block->status = FREE;
    return block->block_size;
}

// status functions

unsigned int heap_free_bytes() {
    return mem_manager->free_bytes;
}

unsigned int heap_used_bytes() {
    return HEAP_SIZE - mem_manager->free_bytes;
}
#endif