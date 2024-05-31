
#include <process.h>

// chequeo en cada timer si se paso del fin del stack para verificar que no haya stack overflow


struct children_info{
    struct childern* first;
    struct childern* last;
};

struct children{
    int pid;
    struct children * next;
};

typedef struct pcb {
    char* name;
    int pid;                    // no tan necesario en el momento de hacer un array de procesos, si es necesario si se hace una lista
    char stdin;
    char stdout;
    char stderr;
    struct sch_info scheduling_info;
    struct children_info childs;
    uint8_t* stack_current;
    uint8_t* stack_end;             // where memory ends
    uint8_t* stack_start;           // where memory starts
    int parent;             // pointer to the information of the parent
    char foreground;
    char alive;
    // array of fd and devices
}pcb;

static int pid =1;
static pcb processes[MAX_PROCESSES_SUPPORTED];

// for syscall fork => return;
int process_create( int pidParent, uint8_t* rip, int argc, char* argv[], int foreground ){
    _cli();
    /*
    pcb_pointer parent;
    if ( pid != 1 )
        parent = &processes[pidParent];
    else
        parent = NULL;
    */
    pcb_pointer process = &processes[pid-1];

    // setting the pcb of the new process
    process->name=argv[0];
    process->pid = pid;
    process->alive = 1;
    process->stdin = STDIN;
    process->stdout = STDOUT;
    process->stderr = STDERR;
    process->foreground = foreground;

    // adding scheduling info
    process->parent = pidParent;
    process->scheduling_info.p_state = READY;
    process->scheduling_info.CPU_time = 0;
    if ( process->pid == 1 )
        process->scheduling_info.priority = ALWAYSACTIVE;
    else
        process->scheduling_info.priority = MOSTIMP;
    

    // TODO: correct
    if (pid != 1){
        pcb parent = processes[pidParent];
        struct children child;
        child.pid = pid;
        child.next = NULL;
        if ( parent.childs.first == NULL ){
            parent.childs.first = &child;
            parent.childs.last = &child;
        }else{
            struct children * current = parent.childs.first;
            while ( current->next != NULL ){
                current = current->next;
            }
            current->next = &child;
            parent.childs.last = &child;
        }
    }else {
        process->childs.first = NULL;
        process->childs.last = NULL;  
    }
                                                    // starts from behind
    uint8_t* stack_end = memalloc(STACK_MEM);      // Saving space for the process' stack
    if ( stack_end == -1 )
        return -1; 
    uint8_t* stack_start = stack_end + STACK_MEM;  // Going to the start of the stack

    process->stack_start = stack_start;
    process->stack_end = stack_end;
    process->stack_current = prepare_process(stack_start, rip, argc, argv);          // At last, we asign the RSP to the registers

    add_process_to_scheduling(process->pid, &processes[(process->pid)-1].scheduling_info,process->stack_current);
    pid++;
    _sti();
    forceTimerTick();
    
    return process->pid;
}

void change_rsp_process( int pid, uint8_t* rsp ){
    processes[pid].stack_current = rsp;
}


// after calling this function must be made a tick interruption
int set_status( int _pid, int newState){

    _cli();

    if ( _pid > pid || _pid < 2 || _pid == 1  )
        return ERROR;
    pcb process = processes[_pid];
    if ( process.alive ){
        if ( newState == process.scheduling_info.p_state )
            return newState;
        if ( newState == BLOCKED ){
            process.scheduling_info.p_state = BLOCKED;
            int result = scheduling_to_blocked(_pid);
            if ( result )
                return ERROR;
            
        }else if ( process.scheduling_info.p_state == BLOCKED ){
            process.scheduling_info.p_state = newState;
            process.scheduling_info.priority = MOSTIMP;
            int result = blocked_to_scheduling(_pid);
            if ( result )
                return ERROR;
        }
        return newState;
    }

    _sti();

    return ERROR;

}


void show_processes(){

    _cli();

    for( int i=0; i < pid-1; i++ ){
        pcb process = processes[i];

        // futuramente imprimir stdin y stdout
        print("PID    NAME    STACK    BASEPR    FOREGROUND");
        enter();
        printDec(process.pid);
        tab();
        print(process.name);
        tab();
        printHex(process.stack_current);
        tab();
        printHex(process.stack_start);
        tab();
        printDec(process.foreground);
        enter();
    }

    _sti();

}

uint8_t* getSchedulingInfo(int pid){
    return &processes[pid].scheduling_info;
}

uint8_t* align_stack( uint8_t* init){
    while( ((int)init%8) != 0 ){
        init= init-1;
    }
    return init;
}

int get_pid_parent(){
    int _pid = get_pid();
    pcb process = processes[_pid];
    if ( process.alive ){
        return process.parent;
    }
    return -1;
}

// returns 0 if eliminated correctly, 1 if error and -1 if not found
int kill_process(int _pid){
    if ( _pid < 0 || _pid > pid )
        return 1;
    
    processes[_pid].alive = 0;
    return delete_process_scheduling(_pid);
}
