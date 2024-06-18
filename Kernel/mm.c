#include <mm.h>

uint8_t* start_mem = MEM_START;
uint8_t* current_mem;
uint8_t* stop_mem = MEM_STOP;

void mem_initialize(){
    current_mem = start_mem;
}


void* memalloc(uint64_t size){
    if ( current_mem == stop_mem || current_mem + size > stop_mem )
        return NULL; // return cant assign
    uint8_t* aux = current_mem;
    current_mem +=size;
    return aux;

}

void free(void* mem){
    //not implemented yet
    return;
}