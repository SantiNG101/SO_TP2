#include <speaker.h>
#include <time.h>

void beep(){
    // uint32_t orFreq = getPITFrq();
    
    playSound(500);
    wait(200);
    stopSound();

    // setPITFrq(orFreq);
}