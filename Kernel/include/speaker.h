#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stdint.h>

extern void playSound(uint32_t freq);
extern void stopSound();

// Make a simple beep, for a 50ms
void beep(void);
void beepCustom(int Hz, int ms);

#endif