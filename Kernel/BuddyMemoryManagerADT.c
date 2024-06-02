#ifdef BUDDY

#include "MemoryManager.h"

typedef struct memChunkCDT
{
    unsigned char free;
    int status;
    memChunkADT next;
    unsigned int size;
} memChunkCDT;

typedef struct memChunkCDT *memChunkADT;

typedef struct memManagerCDT
{
    memChunkADT root;
    memChunkADT last_node;
    unsigned int free_bytes_left;
    unsigned int block_struct_size;
} memManagerCDT;

typedef struct memManagerCDT *memManagerADT;


static memManagerADT get_mem_manager();
static int remove_from_freelist(memManagerADT buddy, memChunkADT block);
static int add_to_freelist(memManagerADT buddy, memChunkADT block, int merge);  
void * mem_alloc(unsigned int size);


static memManagerADT get_mem_manager()
{
    return (memManagerADT)MEM_START;
}

// Returns ERROR if there is an error adding the block to the freelist
static int add_to_freelist(memManagerADT buddy, memChunkADT block, int merge)
{
    // Case: the block is the whole memory
    if (block->size == (MEM_END - MEM_START))
    {
        buddy->root->next = block;
        block->next = buddy->last_node;
        block->free = TRUE;
        return SUCCESS;
    }

    memChunkADT buddy_block = NULL;
    int aux_to_merge = 0;

    if (merge)
    {
        if ((block->status & 0x1) == 1)
        {
            // right block -> least significant bit is 1
            buddy_block = (memChunkADT)(((uint64_t)block) - block->size);
            if (buddy_block->free && buddy_block->size == block->size)
            {
                buddy_block->size *= 2;
                aux_to_merge = 1;
                block = buddy_block;
            }
        }
        else
        {
            // left block
            buddy_block = (memChunkADT)(((uint64_t)block) + block->size);
            if (buddy_block->free && buddy_block->size == block->size)
            {
                buddy_block->size *= 2;
                aux_to_merge = 1;
            }
        }
    }

    if (aux_to_merge)
    {
        block->status = block->status >> 1;
    
        int removed_status = remove_from_freelist(buddy, block);
        if (removed_status == ERROR)
        {
            return ERROR;
        } 
        return add_to_freelist(buddy, block, TRUE);
    }

    memChunkADT current = buddy->root;

    while (current->next->size < block->size)
    {
        current = current->next;
    }

    block->next = current->next;
    current->next = block;
    block->free = TRUE;

    return SUCCESS;
}

// Return ERROR if it's not possible to delete the block
static int remove_from_freelist(memManagerADT buddy, memChunkADT block)
{

    if (block == NULL)
    {
        return ERROR;
    }

    memChunkADT current = buddy->root;

    while (current->next != block && current != NULL)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        return ERROR;
    }
    else
    {
        current->next = current->next->next;
        return SUCCESS;
    }
}


memManagerADT mem_initialize()
{

    memManagerADT buddy = (memManagerADT)MEM_START;

    buddy->free_bytes_left = (MEM_END - MEM_START) + 1;
    buddy->block_struct_size = ((sizeof(memChunkCDT) + (BYTE_ALIGMENT - 1)) & ~MASK_BYTE_ALIGMENT);


    memChunkADT root = (memChunkADT)(MEM_START + sizeof(memManagerCDT) + 1);
    buddy->root->next = (void *) root;
    buddy->root->size = 0;
    buddy->last_node->next = NULL;
    buddy->last_node->size = buddy->free_bytes_left;
    root->size = buddy->free_bytes_left;
    root->next = (void *) &buddy->last_node;
    root->free = TRUE;
    root->status = 0;

    return buddy;
}

// Returns ERROR if there is an error when allocating memory
void * mem_alloc(unsigned int size)
{

    memManagerADT buddy = get_mem_manager();

    if (size == 0)
    {
        return ERROR;
    }

    memChunkADT current, prev;
    void *allocated_memory = NULL;

    size += buddy->block_struct_size;

    if ((size & MASK_BYTE_ALIGMENT) != 0)
    {
        size += (BYTE_ALIGMENT - (size & MASK_BYTE_ALIGMENT));
    }

    if (size > buddy->free_bytes_left)
    {
        return ERROR;
    }

    prev = buddy->root;
    current = buddy->root->next;

    while ((current->size < size) && (current->next != NULL))
    {
        prev = current;
        current = current->next;
    }

    if (current == buddy->last_node)
    {
        return ERROR;
    }

    allocated_memory = (void *)(((uint8_t *)prev->next) + buddy->block_struct_size);

    prev->next = current->next;

    while (current->size / 2 >= MIN_BLOCK_SIZE && current->size / 2 >= size)
    {
        current->size /= 2;
        current->status = current->status << 1;
        memChunkADT new = (void *)(((uint64_t)current) + current->size);
        new->size = current->size;
        new->free = TRUE;
        new->status = current->status | 0x1;
        add_to_freelist(buddy, new, FALSE);
    }

    buddy->free_bytes_left -= current->size;
    current->free = FALSE;

    return allocated_memory;
}

int free(unsigned int allocated_memory)
{

    memManagerADT buddy = get_mem_manager();

    if (allocated_memory == NULL)
    {
        return ERROR;
    }
    uint8_t *mem_to_free = ((uint8_t *)allocated_memory) - buddy->block_struct_size;
    memChunkADT block_to_free = (void *)mem_to_free;

    int freed_bytes = block_to_free->size;

    add_to_freelist(buddy, block_to_free, TRUE);

    buddy->free_bytes_left += freed_bytes;

    return SUCCESS;
}

#endif