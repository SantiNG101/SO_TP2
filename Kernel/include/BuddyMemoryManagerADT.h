#ifndef _BUDDY_MEMORY_MANAGER_ADT_H_
#define _BUDDY_MEMORY_MANAGER_ADT_H_

#include <stddef.h>
#include <stdint.h>

#define ERROR -1
#define SUCCESS 0
#define MIN_BLOCK_SIZE 512
#define MEM_START 0x50000
#define MEM_END 0x9FFFF
#define BYTE_ALIGMENT 8
#define MASK_BYTE_ALIGMENT 0x07
#define TRUE 1
#define FALSE 0

typedef struct MemBlockCDT *MemBlockADT;
typedef struct BuddyMemoryManagmentCDT *BuddyMemoryManagmentADT;

BuddyMemoryManagmentADT buddy_initialize();
void * buddy_alloc(BuddyMemoryManagmentADT buddy, unsigned int size);
void buddy_free(BuddyMemoryManagmentADT buddy, void *allocated_memory);

#endif