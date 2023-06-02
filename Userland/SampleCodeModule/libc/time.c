#include "include/time.h"
//         0123 45  67    89    1011    1213
//long int yyyy mm  dd    hh    MM      ss
uint64_t timestam(){
    return 20020414023010;
}

int getDate(int * day, int * month, int * year){
     uint64_t ts = timestam();
     *year = ts/10000000000;
     *month = (ts/100000000)%100;
     *day =  (ts/1000000)%100;
    return 1;
}

int getTime(int * hour, int * min, int * seconds){
    uint64_t ts = timestam();
    *hour = (ts%1000000)/10000;
    *min = (ts%10000)/100;
    *seconds = ts % 100;
    return 1;
}