#ifndef _BITMAP_MEMORY_MANAGER_ADT_H_
#define _BITMAP_MEMORY_MANAGER_ADT_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define MEM_START 0x50000;
#define MEM_END 0x9FFFF;
#define ERROR -1
#define CHUNK_SIZE 4096 // tbd
#define MAX_CHUNKS (MEM_END - MEM_START) / CHUNK_SIZE

#define TRUE 1
#define FALSE 0

memManamemManagerADT mem_initialize(int chunk_size);
void * mem_alloc(memManagerADT mem_manager, int size);
void * free(memManagerADT mem_manager, void * allocated_memory);

#endif