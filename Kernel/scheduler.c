
#include "include/scheduler.h"


typedef struct listHeader{
    p_list first;
}listHeader;

typedef struct list{
    int pid;
    uint8_t* stack_pointer;
    struct sch_info * process_info;
    p_list next;
}list;



static listHeader priority[4];
static int is_first = 1;
static p_list running = NULL;
static listHeader blocked;

void initialize_scheduler(){

    ncPrint("Initializing scheduler:");
    ncNewline();
    priority[0].first = NULL;
    priority[1].first = NULL;
    priority[2].first = NULL;
    priority[3].first = NULL;

}

void add_process_to_scheduling( int pid, struct sch_info * process_info, uint8_t* rsp){


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

void delete_process_scheduling( int pid ){
    p_list aux;

}


uint8_t* schedule( uint8_t* actual_pointer){

    if ( running != NULL ){
        running->stack_pointer = actual_pointer;
        change_rsp_process(running->pid,actual_pointer);
        running->process_info->CPU_time++;
        if ( running->process_info->p_state == RUNNING )
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
    running = toRun;

    return toRun->stack_pointer;
    
}

p_list look_in_list ( int lvl , p_list from ){
    p_list starter = NULL;
    int around = 0;

    if ( from != NULL ){
        starter = from;
    } else {
        starter = priority[lvl].first;
    }
    if ( starter != NULL ){
        p_list current = starter->next;
        while( current != NULL && !around ){
            if ( current->process_info->p_state == READY )
                return current;
            current = current->next;
            if ( current == starter )
                around++;
        }
    }
    
    return NULL;
}

p_list search_from_0(){

    p_list toReturn = NULL;

    toReturn = look_in_list(MOSTIMP, running);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(MEDIUMIMP, priority[MEDIUMIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(LESSIMP, priority[LESSIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    return priority[ALWAYSACTIVE].first;

}

p_list search_from_1(){
    p_list toReturn = NULL;

    toReturn = look_in_list(MOSTIMP, priority[MOSTIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(MEDIUMIMP, running);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(LESSIMP, priority[LESSIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    return priority[ALWAYSACTIVE].first;
}

p_list search_from_2(){
    p_list toReturn = NULL;

    toReturn = look_in_list(MOSTIMP, priority[MOSTIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(MEDIUMIMP, priority[MEDIUMIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(LESSIMP, running);
    if ( toReturn != NULL )
        return toReturn;
    return priority[ALWAYSACTIVE].first;
}

p_list search_from_top(){
    p_list toReturn = NULL;

    toReturn = look_in_list(MOSTIMP, priority[MOSTIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(MEDIUMIMP, priority[MEDIUMIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    toReturn = look_in_list(LESSIMP, priority[LESSIMP].first);
    if ( toReturn != NULL )
        return toReturn;
    return priority[ALWAYSACTIVE].first;
}

p_list getReadyToRun(){
    
    if ( running == NULL ){
        return priority[3].first;
    }

    int prio_lvl = running->process_info->priority;

    switch (prio_lvl){
        case 0:
        return search_from_0();
        case 1:
        return search_from_1();
        case 2: 
        return search_from_2();
        default:
        return search_from_top();
    }
}

// returns 1 in error and 0 in success
int scheduling_to_blocked(){
    if ( running == NULL ){
        return 1;
    }
    int lvl = running->process_info->priority;


    p_list current = priority[lvl].first;
    if ( current == NULL )
        return 1;
    if ( current == running && (current->next == NULL || current->next == current) ){
        priority[lvl].first = NULL;
    } else {
        while( current->next != running ){
            current = current->next;
        }
        current->next = running->next;
    }
    
    // add blocked programs
    if ( blocked.first == NULL ){
        blocked.first = running;
    }else{
        p_list current_blocked = blocked.first;
        while( current_blocked->next != NULL ){
            current_blocked = current_blocked->next;
        }
        current_blocked->next = running;
    }
    return 0;
}

// returns 1 in error and 0 in success
int blocked_to_scheduling( int _pid){
    if ( blocked.first == NULL ){
        return 1;
    }
    p_list current = blocked.first;
    p_list aux = current;
    if ( current->pid == _pid ){
        blocked.first = NULL;
    }else {
        while ( current->next == NULL || current->next->pid != _pid ){
            current = current->next;
        }
        aux = current;
        if ( current ->next == NULL )
            return 1;
        current->next = current->next->next;
    }

    // move to the scheduling table
    int lvl = aux->process_info->priority;

    p_list aux2 = priority[lvl].first;
    // if there is no first process next points to itself
    if ( aux2 == NULL ){
        priority[lvl].first = aux;
        aux->next = aux;
        return 0;
    }

    while( aux2->next != priority[lvl].first ){
        aux2 = aux2->next;
    }
    aux2->next = aux;
    aux->next = priority[lvl].first;

    return 0;
}


int get_pid(){
    if ( running == NULL )
        return -1;
    else
        return running->pid;
}