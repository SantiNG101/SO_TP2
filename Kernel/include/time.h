//
// Created by ptorl on 5/10/2023.
//

#ifndef KERNEL_TIME_H
#define KERNEL_TIME_H
unsigned  int seconds();
unsigned int minutes();
unsigned int hours();
unsigned int year();
unsigned int day();
unsigned int month();
void timeToStr(char * dest);
void dateToStr(char * dest);
#endif //KERNEL_TIME_H
