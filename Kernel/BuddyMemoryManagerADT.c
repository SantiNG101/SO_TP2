#ifndef BUDDY

#include "BuddyMemoryManagerADT.h"

typedef struct MemBlockCDT
{
    unsigned char free;
    int status;
    MemBlockADT next;
    unsigned int size;
} MemBlockCDT;

typedef struct MemBlockCDT *MemBlockADT;

typedef struct BuddyMemoryManagmentCDT
{
    MemBlockADT root;
    MemBlockADT last_node;
    unsigned int free_bytes_left;
    unsigned int block_struct_size;
} BuddyMemoryManagmentCDT;

typedef struct BuddyMemoryManagmentCDT *BuddyMemoryManagmentADT;

static int remove_from_freelist(BuddyMemoryManagmentADT buddy, MemBlockADT block);
static int add_to_freelist(BuddyMemoryManagmentADT buddy, MemBlockADT block, int merge);  
void *buddy_alloc(BuddyMemoryManagmentADT buddy, unsigned int size);


// Returns ERROR if there is an error adding the block to the freelist
static int add_to_freelist(BuddyMemoryManagmentADT buddy, MemBlockADT block, int merge)
{
    // Case: the block is the whole memory
    if (block->size == (MEM_END - MEM_START))
    {
        buddy->root->next = block;
        block->next = buddy->last_node;
        block->free = TRUE;
        return SUCCESS;
    }

    MemBlockADT buddy_block = NULL;
    int aux_to_merge = 0;

    if (merge)
    {
        if ((block->status & 0x1) == 1)
        {
            // right block -> least significant bit is 1
            buddy_block = (MemBlockADT)(((uint64_t)block) - block->size);
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
            buddy_block = (MemBlockADT)(((uint64_t)block) + block->size);
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

    MemBlockADT current = buddy->root;

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
static int remove_from_freelist(BuddyMemoryManagmentADT buddy, MemBlockADT block)
{

    if (block == NULL)
    {
        return ERROR;
    }

    MemBlockADT current = buddy->root;

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


BuddyMemoryManagmentADT buddy_initialize()
{

    BuddyMemoryManagmentADT buddy = (BuddyMemoryManagmentADT)MEM_START;

    buddy->free_bytes_left = (MEM_END - MEM_START) + 1;
    buddy->block_struct_size = ((sizeof(MemBlockCDT) + (BYTE_ALIGMENT - 1)) & ~MASK_BYTE_ALIGMENT);


    MemBlockADT root = (MemBlockADT)(MEM_START + sizeof(BuddyMemoryManagmentCDT) + 1);
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

// Returns NULL if there is an error when allocating memory
void *buddy_alloc(BuddyMemoryManagmentADT buddy, unsigned int size)
{

    if (size == 0)
    {
        return NULL;
    }

    MemBlockADT current, prev;
    void *allocated_memory = NULL;

    size += buddy->block_struct_size;

    if ((size & MASK_BYTE_ALIGMENT) != 0)
    {
        size += (BYTE_ALIGMENT - (size & MASK_BYTE_ALIGMENT));
    }

    if (size > buddy->free_bytes_left)
    {
        return NULL;
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
        return NULL;
    }

    allocated_memory = (void *)(((uint8_t *)prev->next) + buddy->block_struct_size);

    prev->next = current->next;

    while (current->size / 2 >= MIN_BLOCK_SIZE && current->size / 2 >= size)
    {
        current->size /= 2;
        current->status = current->status << 1;
        MemBlockADT new = (void *)(((uint64_t)current) + current->size);
        new->size = current->size;
        new->free = TRUE;
        new->status = current->status | 0x1;
        add_to_freelist(buddy, new, FALSE);
    }

    buddy->free_bytes_left -= current->size;
    current->free = FALSE;

    return allocated_memory;
}

void buddy_free(BuddyMemoryManagmentADT buddy, void *allocated_memory)
{

    if (allocated_memory == NULL)
    {
        return;
    }
    uint8_t *mem_to_free = ((uint8_t *)allocated_memory) - buddy->block_struct_size;
    MemBlockADT block_to_free = (void *)mem_to_free;

    int freed_bytes = block_to_free->size;

    add_to_freelist(buddy, block_to_free, TRUE);

    buddy->free_bytes_left += freed_bytes;
}


#endif