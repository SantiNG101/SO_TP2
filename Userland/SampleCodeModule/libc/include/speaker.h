#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stdint.h>

extern void playSound(uint64_t frq, uint64_t time);
extern void startSound(uint64_t frq);
extern void stopSound();


#endif