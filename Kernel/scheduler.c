
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


uint8_t* schedule( uint8_t* actual_pointer){

    _cli();

    if ( running != NULL ){
        running->stack_pointer = actual_pointer;
        change_rsp_process(running->pid,actual_pointer);
        if ( running->process_info->p_state == RUNNING ){
            running->process_info->p_state = READY;
            running->process_info->CPU_time++;
        }
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
    if ( toRun == NULL ){
        return actual_pointer;
    }
        
    toRun->process_info->p_state=RUNNING;
    running = toRun;

    _sti();

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

p_list remove_in_scheduling_by_level( int _pid, int lvl ){
    p_list toReturn = NULL;
    if ( lvl > 2 || lvl < 0 )
        return toReturn;
    p_list current = priority[lvl].first;
    if ( current != NULL ) {
        if ( current->pid == _pid ){
            toReturn = current;
            priority[lvl].first = current->next;
            p_list aux = current->next;
            while( current != aux->next ){
                aux = aux->next;
            }
            aux->next = priority[lvl].first;
        }else{
            while(current->next != NULL || current->next->pid == _pid){
                current = current->next;
            }
            if ( current->next->pid == _pid ){
                toReturn = current->next;
                current->next = current->next->next;
            }
        }
    }
    return toReturn;
}

p_list find_with_remove( int _pid ){
    p_list result = remove_in_scheduling_by_level( _pid, MOSTIMP );
    if ( result != NULL ){
        return result;
    }
    result = remove_in_scheduling_by_level( _pid, MEDIUMIMP );
    if ( result != NULL ){
        return result;
    }
    result = remove_in_scheduling_by_level( _pid, LESSIMP );
    return result;
}

int add_to_priority_list( p_list process ){

    if ( process == NULL )
        return ERROR;
    
    int lvl = process->process_info->priority;
    p_list current = priority[lvl].first;
    if ( current == NULL ){
        priority[lvl].first = process;
        process->next == process;
    }else {
        while( current->next != priority[lvl].first ){
            current = current->next;
        }
        current->next = process;
        process->next = priority[lvl].first;
    }
    return SUCCESS;

}



int change_priority( int _pid, int new_priority ){
    p_list process = find_with_remove(_pid);
    if ( process == NULL ){
        return ERROR;
    }
    process->process_info->priority = new_priority;

    return add_to_priority_list(process); 


}

int delete_process_scheduling( int _pid ){
    
    p_list toReturn = find_with_remove(_pid);
    return toReturn==NULL?ERROR:SUCCESS;

}

// returns 1 in error and 0 in success
int scheduling_to_blocked(int _pid){
    p_list process = find_with_remove(_pid);
    
    // add blocked programs
    if ( blocked.first == NULL ){
        blocked.first = process;
    }else{
        p_list current_blocked = blocked.first;
        while( current_blocked->next != NULL ){
            current_blocked = current_blocked->next;
        }
        current_blocked->next = process;
    }
    return SUCCESS;
}

// returns 1 in error and 0 in success
int blocked_to_scheduling( int _pid){
    if ( blocked.first == NULL ){
        return ERROR;
    }
    p_list current = blocked.first;
    p_list aux = current;
    if ( current->pid == _pid ){
        blocked.first = NULL;
    }else {
        while ( current->next != NULL || current->next->pid != _pid ){
            current = current->next;
        }
        aux = current;
        if ( current ->next == NULL )
            return ERROR;
        current->next = current->next->next;
    }

    // move to the scheduling table
    int lvl = aux->process_info->priority;

    p_list aux2 = priority[lvl].first;
    // if there is no first process next points to itself
    if ( aux2 == NULL ){
        priority[lvl].first = aux;
        aux->next = aux;
        return SUCCESS;
    }

    while( aux2->next != priority[lvl].first ){
        aux2 = aux2->next;
    }
    aux2->next = aux;
    aux->next = priority[lvl].first;

    return SUCCESS;
}


int get_pid(){
    if ( running == NULL )
        return NOTFOUND;
    else
        return running->pid;
}