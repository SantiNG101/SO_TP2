#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>
#include <stdint.h>

//Seccion memoria
void* myMalloc(size_t size);
void myFree(void* address);

uint16_t atoi(char * str);
unsigned int mi_rand();
void srand(unsigned int seed);
int64_t initPhyloReunion(int argc, char* argv[]);
#endif
