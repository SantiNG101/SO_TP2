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

void marioTheme(){
    beepCustom(659, 125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(523, 125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(784, 125);
    wait(375);
    beepCustom(392, 125);
    wait(375);
    beepCustom(523, 125);
    wait(250);
    beepCustom(392, 125);
    wait(250);
    beepCustom(330, 125);
    wait(250);
    beepCustom(440, 125);
    wait(125);
    beepCustom(494, 125);
    wait(125);
    beepCustom(466, 125);
    wait(42);
    beepCustom(440, 125);
    wait(125);
    beepCustom(392, 125);
    wait(125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(784, 125);
    wait(125);
    beepCustom(880, 125);
    wait(125);
    beepCustom(698, 125);
    beepCustom(784, 125);
    wait(125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(587, 125);
    beepCustom(494, 125);
    wait(125);
    beepCustom(523, 125);
    wait(250);
    beepCustom(392, 125);
    wait(250);
    beepCustom(330, 125);
    wait(250);
    beepCustom(440, 125);
    wait(125);
    beepCustom(494, 125);
    wait(125);
    beepCustom(466, 125);
    wait(42);
    beepCustom(440, 125);
    wait(125);
    beepCustom(392, 125);
    wait(125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(784, 125);
    wait(125);
    beepCustom(880, 125);
    wait(125);
    beepCustom(698, 125);
    beepCustom(784, 125);
    wait(125);
    beepCustom(659, 125);
    wait(125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(587, 125);
    beepCustom(494, 125);
    wait(375);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(415, 125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    beepCustom(587, 125);
    wait(250);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(698, 125);
    wait(125);
    beepCustom(698, 125);
    beepCustom(698, 125);
    wait(625);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(415, 125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    beepCustom(587, 125);
    wait(250);
    beepCustom(622, 125);
    wait(250);
    beepCustom(587, 125);
    wait(250);
    beepCustom(523, 125);
    wait(1125);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(415, 125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    beepCustom(587, 125);
    wait(250);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(698, 125);
    wait(125);
    beepCustom(698, 125);
    beepCustom(698, 125);
    wait(625);
    beepCustom(784, 125);
    beepCustom(740, 125);
    beepCustom(698, 125);
    wait(42);
    beepCustom(622, 125);
    wait(125);
    beepCustom(659, 125);
    wait(167);
    beepCustom(415, 125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    wait(125);
    beepCustom(440, 125);
    beepCustom(523, 125);
    beepCustom(587, 125);
    wait(250);
    beepCustom(622, 125);
    wait(250);
    beepCustom(587, 125);
    wait(250);
    beepCustom(523, 125);
}

void imperialMarch(){
    beepCustom(440,500);

    beepCustom(440,500);

    beepCustom(440,500);

    beepCustom(349,350);

    beepCustom(523,150);

    beepCustom(440,500);

    beepCustom(349,350);

    beepCustom(523,150);

    beepCustom(440,1000);

    beepCustom(659,500);

    beepCustom(659,500);

    beepCustom(659,500);

    beepCustom(698,350);

    beepCustom(523,150);

    beepCustom(415,500);

    beepCustom(349,350);

    beepCustom(523,150);

    beepCustom(440,1000);
}

void tetris(){
    beepCustom(658, 125);
    beepCustom(1320, 500);
    beepCustom(990, 250);
    beepCustom(1056, 250);
    beepCustom(1188, 250);
    beepCustom(1320, 125);
    beepCustom(1188, 125);
    beepCustom(1056, 250);
    beepCustom(990, 250);
    beepCustom(880, 500);
    beepCustom(880, 250);
    beepCustom(1056, 250);
    beepCustom(1320, 500);
    beepCustom(1188, 250);
    beepCustom(1056, 250);
    beepCustom(990, 750);
    beepCustom(1056, 250);
    beepCustom(1188, 500);
    beepCustom(1320, 500);
    beepCustom(1056, 500);
    beepCustom(880, 500);
    beepCustom(880, 500);
    wait(250);
    beepCustom(1188, 500);
    beepCustom(1408, 250);
    beepCustom(1760, 500);
    beepCustom(1584, 250);
    beepCustom(1408, 250);
    beepCustom(1320, 750);
    beepCustom(1056, 250);
    beepCustom(1320, 500);
    beepCustom(1188, 250);
    beepCustom(1056, 250);
    beepCustom(990, 500);
    beepCustom(990, 250);
    beepCustom(1056, 250);
    beepCustom(1188, 500);
    beepCustom(1320, 500);
    beepCustom(1056, 500);
    beepCustom(880, 500);
    beepCustom(880, 500);
    wait(500);
    beepCustom(1320, 500);
    beepCustom(990, 250);
    beepCustom(1056, 250);
    beepCustom(1188, 250);
    beepCustom(1320, 125);
    beepCustom(1188, 125);
    beepCustom(1056, 250);
    beepCustom(990, 250);
    beepCustom(880, 500);
    beepCustom(880, 250);
    beepCustom(1056, 250);
    beepCustom(1320, 500);
    beepCustom(1188, 250);
    beepCustom(1056, 250);
    beepCustom(990, 750);
    beepCustom(1056, 250);
    beepCustom(1188, 500);
    beepCustom(1320, 500);
    beepCustom(1056, 500);
    beepCustom(880, 500);
    beepCustom(880, 500);
    wait(250);
    beepCustom(1188, 500);
    beepCustom(1408, 250);
    beepCustom(1760, 500);
    beepCustom(1584, 250);
    beepCustom(1408, 250);
    beepCustom(1320, 750);
    beepCustom(1056, 250);
    beepCustom(1320, 500);
    beepCustom(1188, 250);
    beepCustom(1056, 250);
    beepCustom(990, 500);
    beepCustom(990, 250);
    beepCustom(1056, 250);
    beepCustom(1188, 500);
    beepCustom(1320, 500);
    beepCustom(1056, 500);
    beepCustom(880, 500);
    beepCustom(880, 500);
    wait(500);
    beepCustom(660, 1000);
    beepCustom(528, 1000);
    beepCustom(594, 1000);
    beepCustom(495, 1000);
    beepCustom(528, 1000);
    beepCustom(440, 1000);
    beepCustom(419, 1000);
    beepCustom(495, 1000);
    beepCustom(660, 1000);
    beepCustom(528, 1000);
    beepCustom(594, 1000);
    beepCustom(495, 1000);
    beepCustom(528, 500);
    beepCustom(660, 500);
    beepCustom(880, 1000);
    beepCustom(838, 2000);
    beepCustom(660, 1000);
    beepCustom(528, 1000);
    beepCustom(594, 1000);
    beepCustom(495, 1000);
    beepCustom(528, 1000);
    beepCustom(440, 1000);
    beepCustom(419, 1000);
    beepCustom(495, 1000);
    beepCustom(660, 1000);
    beepCustom(528, 1000);
    beepCustom(594, 1000);
    beepCustom(495, 1000);
    beepCustom(528, 500);
    beepCustom(660, 500);
    beepCustom(880, 1000);
    beepCustom(838, 2000);
}

void songOfStorms(void){
    beepCustom(D4,200);
    beepCustom(A4,200);
    beepCustom(D5,200);
    wait(300);
    beepCustom(D4,200);
    beepCustom(A4,200);
    beepCustom(D5,200);
    wait(300);
    beepCustom(E5,200);
    beepCustom(F5,200);
    beepCustom(E5,200);
    beepCustom(F5,200);
    beepCustom(E5,200);
    beepCustom(C5,200);
    beepCustom(A4,200);
    wait(50);
    beepCustom(A4,200);
    wait(300);
    beepCustom(D4,200);
    beepCustom(F4,200);
    beepCustom(G4,300);
    beepCustom(A4,200);
    wait(50);
    beepCustom(A4,200);
    wait(300);
    beepCustom(D4,200);
    beepCustom(F4,200);
    beepCustom(G4,200);
    beepCustom(E4,300);
}
