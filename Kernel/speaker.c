#include <speaker.h>
#include <time.h>

void beepCustom(int Hz, int ms){
    playSound(Hz);
    wait(ms);
    stopSound();
}

void beep(){
    beepCustom(300,200);
}
