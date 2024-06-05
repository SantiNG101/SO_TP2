#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#define ERROR -1
#define SUCCESS 0

#define MEM_START 0x50000
#define MEM_END 0x9FFFF

#define BYTE_ALIGMENT 8
#define MASK_BYTE_ALIGMENT 0x07

#define TRUE 1
#define FALSE 0

#define CHUNK_SIZE 4096 // 2^12
#define MIN_BLOCK_SIZE 512
#define MAX_CHUNKS (MEM_END - MEM_START) / CHUNK_SIZE

typedef struct memManagerCDT *memManagerADT;
typedef struct memChunkCDT *memChunkADT;

void mem_initialize();
void * mem_alloc(unsigned int size);
void free(unsigned int allocated_memory);

#endif