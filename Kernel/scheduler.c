
#include "include/scheduler.h"


char* scheduling_table_dir = 0x900000;
char* current_dir;

typedef struct entry{
    uint64_t pid; // process
    uint8_t stack_pointer;
    int status; // 0 blockeado | 1 ready | 2 running
}entry;

void initialize_scheduler(){
    
    // asigno el espacio para varios procesos
    //scheduling_table_dir = memalloc(sizeof(entry)*10);

    //creo el proceso 1
    entry *init  = (entry *) scheduling_table_dir; 
    current_dir += (int) scheduling_table_dir + sizeof(entry*);
}

/*
void* schedule( char* actual_pointer){

    int chose = 0;
    entry* local_mem = find_running();
    local_mem->status = READY;
    local_mem->stack_pointer = actual_pointer;

    local_mem += (int) local_mem+sizeof(entry*);
    // int priority_level = -1;
    while( !chose ){
        while( local_mem != current_dir ){
            if (local_mem->status == READY)
                return local_mem;
            local_mem += (int) local_mem+sizeof(entry*);
        }
        local_mem = scheduling_table_dir;
    }
    
    // no busywaiting, tengo un proceso siempre ready que en caso de que esten todos bloqueados 
    // => elijo este que hace en un while 1 hlt => para esperar una interrupcion
}
*/