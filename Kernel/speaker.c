#include <speaker.h>
#include <time.h>

//Llama a las respectivas interrupciones para generar sonido
void beepCustom(int Hz, int ms){
    playSound(Hz);
    wait(ms);
    stopSound();
}

void beep(){
    beepCustom(300,200);
}
