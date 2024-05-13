#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>
#include <stdarg.h>

#define MEM_START 0x50000;
#define MEM_STOP 0x9FFFF;


void mem_initialize();
void* memalloc(int size);
void free(void* mem);

#endif