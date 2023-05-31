#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stdint.h>

void playSound(uint32_t freq);
void stopSound();

// Make a simple beep, for a 50ms
void beep(void);
void marioTheme(void);
void imperialMarch(void);
void tetris(void);

#endif