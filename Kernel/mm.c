#ifdef MM
#include <mm.h>

/*
typedef struct mem_block{
    char* start;
    char* end;
    char used;
}block;

void* memalloc(uint64_t size);
void free(void* mem);

char memarray[521142272] = {0};
*/

char* start_mem = MEM_START;
char* current_mem;
char* stop_mem = MEM_STOP;

void mem_initialize(){
    current_mem = start_mem;
}


void* memalloc(uint64_t size){
    if ( current_mem == stop_mem || current_mem + size > stop_mem )
        return -1; // return cant assign
    char* aux = current_mem;
    current_mem +=size;
    return aux;

}

void free(void* mem){
    //not implemented yet
    return;
}
#endif