#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <strings.h>
#include <time.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
void* myMalloc(size_t size);
void myFree(void* address);
void cleanActualRegisters();

#endif