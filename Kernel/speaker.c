#include <speaker.h>
#include <time.h>

void beepCustom(int Hz, int ms){
    playSound(Hz);
    wait(ms);
    stopSound();
}

void beep(){
    // uint32_t orFreq = getPITFrq();

    beepCustom(300,200);

    // setPITFrq(orFreq);
}
