//
// Created by ptorl on 5/10/2023.
//

#ifndef KERNEL_TIME_H
#define KERNEL_TIME_H
#define _MS_BETWEEN_TICKS_ 55

#include <stdint.h>

unsigned  int seconds();
unsigned int minutes();
unsigned int hours();
unsigned int year();
unsigned int day();
unsigned int month();
void timeToStr(char * dest);
void dateToStr(char * dest);

/* ------- TICK ------- */
void tick(void);
int getTicks();
int getElapsedMilliseconds();
int getElapsedSeconds();

/* ------ TIMER ------ */
int wait(uint64_t ms);

#endif //KERNEL_TIME_H
