
#include "include/scheduler.h"


typedef struct listHeader{
    p_list first;
    p_list running;
}listHeader;

typedef struct list{
    int pid;
    uint8_t* stack_pointer;
    struct sch_info * process_info;
    p_list next;
}list;



static listHeader priority[4];
static int is_first = 1;

void initialize_scheduler(){

    ncPrint("Initializing scheduler:");
    ncNewline();
    priority[0].first = NULL;
    priority[0].running = NULL;
    priority[1].first = NULL;
    priority[1].running = NULL;
    priority[2].first = NULL;
    priority[2].running = NULL;
    priority[3].first = NULL;
    priority[3].running = NULL;

}

void addProcessToScheduling( int pid, struct sch_info * process_info, uint8_t* rsp){


    p_list process = memalloc(sizeof( struct list ));
    process->pid = pid;
    process->stack_pointer = rsp;
    process->process_info = process_info;

    // null checks

    p_list aux = priority[process_info->priority].first;
    // if there is no first process next points to itself
    if ( aux == NULL ){
        priority[process_info->priority].first = process;
        process->next = process;
        return;
    }

    while( aux->next != priority[process_info->priority].first ){
        aux = aux->next;
    }
    aux->next = process;

    process->next = priority[process_info->priority].first;
    return;

}

void deleteProcessScheduling( int pid ){
    p_list aux;

}


uint8_t* schedule( uint8_t* actual_pointer){

    p_list running = getRunning();
    if ( running != NULL ){
        running->stack_pointer = actual_pointer;
        running->process_info->CPU_time++;
        running->process_info->p_state = READY;
    }

    /*
    if ( is_first){
        p_list init = priority[3].first;
        init->process_info->p_state = RUNNING;
        init->stack_pointer = actual_pointer;
        is_first--;
        return init->stack_pointer;
    }
    */
    // 
    //p_list priority1 = priority[0].running;
    /*
    if ( priority1 != NULL ){
        if ( priority1->next == priority[ priority1->process_info->priority ].first ){
        rearrangePriorities();
        }
    }
    */
    p_list toRun = getReadyToRun();
    if ( toRun == NULL )
        return actual_pointer;
    toRun->process_info->p_state=RUNNING;
    priority[toRun->process_info->priority].running = toRun;

    return toRun->stack_pointer;
    
}


// A blocking function call this function to change the process for it to wait
void blocking( uint8_t* actual_pointer ){
    p_list running = getRunning();
    if ( running != NULL ){
        running->stack_pointer = actual_pointer;
        running->process_info->p_state = BLOCKED;
        schedule( actual_pointer );
    }
    schedule( actual_pointer );
}

// function called by timer tick to get rid of a running process
uint8_t* from_tick( uint8_t* actual_pointer ){
    p_list running = getRunning();
    if ( running != NULL ){
        running->stack_pointer = actual_pointer;
        running->process_info->CPU_time++;
        running->process_info->p_state = READY;
    }
    return schedule( actual_pointer );
}

p_list getRunning(){
    if ( priority[0].running && (priority[0].running->process_info->p_state == RUNNING) )
        return priority[0].running;
    if ( priority[1].running && (priority[1].running->process_info->p_state == RUNNING) )
        return priority[1].running;
    if ( priority[2].running && ( priority[2].running->process_info->p_state == RUNNING) )
        return priority[2].running;
    if ( priority[3].running && ( priority[3].running->process_info->p_state == RUNNING) )
        return priority[3].running;
    return NULL;
}

p_list getReadyToRun(){
    p_list starter =  priority[0].running;
    if (starter != NULL ){
        p_list current =  starter->next;
        while( starter != current ){
            if ( current->process_info->p_state == READY )
                return current;
            current = current->next;
        }
    }

    starter = priority[1].running;
    if ( starter != NULL ){
        p_list current =  starter->next;
        while( starter != current ){
            if ( current->process_info->p_state == READY )
                return current;
            current = current->next;
        }
    }

    starter = priority[2].running;
    if ( starter != NULL ){
        p_list current =  starter->next;
        while( starter != current ){
            if ( current->process_info->p_state == READY )
                return current;
            current = current->next;
        }
    }

    return priority[3].first;
}