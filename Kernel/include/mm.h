#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#define MEM_START 0xA00000
#define HEAP_START 0x1F000000
#define MEM_STOP 0x1F600000
#define BLOCK_SIZE 4096 // 2^12
#define MEM_SIZE (MEM_STOP - MEM_START)
#define HEAP_SIZE (MEM_STOP - HEAP_START)
#define NUM_BLOCKS (HEAP_SIZE / BLOCK_SIZE)
#define BOUNDARY_BLOCK 2
#define MIN_BLOCK_SIZE 512

#define FREE 1
#define USED 0

void mem_initialize();
void * memalloc(uint64_t size);
void free(void* mem);
unsigned int heap_free_bytes();
unsigned int heap_used_bytes();

#endif