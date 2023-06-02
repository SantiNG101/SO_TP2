#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

/*
 * Performs a timer_wait for given ms
 */
extern void sleep(uint64_t ms);

/*
 * Returns a timestamp with the current system time. It can be perfectly used as to get the current date and/or hour.
 */

 /*
  * yyyy mm dd hhMMss
  * long int yyyymmddhhMMss
  */
extern int timestamp();

/*
 * Returns how many ticks has been done since the Kernel Booted.
 */
extern int ticks();


// Another functions made out of the first three.

// int getTime();
int getDate(int * day, int * hour, int * year);
// int getDateTime();

#endif