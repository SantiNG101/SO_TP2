#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdarg.h>
#include <strings.h>
#include <time.h>
#include <screen.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

//asm std
char *cpuVendor(char *result);
void cleanActualRegisters();

//Seccion memoria
void* myMalloc(size_t size);
void myFree(void* address);


//Seccion puts / printf
int puts(const char * string);
int putChar(int c);
int printf(const char * format, ...);

//Seccion gets / scanf

#endif