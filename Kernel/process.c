
#include <process.h>

// chequeo en cada timer si se paso del fin del stack para verificar que no haya stack overflow


struct children_info{
    struct childern* first;
    struct childern* last;
};

typedef struct children{
    int pid;
    struct children * next;
}* p_children;

typedef struct pcb {
    char* name;
    int pid;                        // no tan necesario en el momento de hacer un array de procesos, si es necesario si se hace una lista
    char fd[3];
    struct sch_info * scheduling_info;    // cambiar a puntero
    struct children_info childs;        // cambiar a puntero para sacar el warning, implica asignar mem
    uint8_t* stack_current;
    uint8_t* stack_end;             // where memory ends
    uint8_t* stack_start;           // where memory starts
    int parent;                     // pointer to the information of the parent
    char foreground;
    char alive;
}pcb;

static int pid =1;
static pcb_pointer processes[MAX_PROCESSES_SUPPORTED];

// for syscall fork => return;
int process_create( int pidParent, uint8_t* rip, int argc, char* argv[], int foreground ){
    _cli();

    if ( pid > MAX_PROCESSES_SUPPORTED )
        return -1;
    /*
    pcb_pointer parent;
    if ( pid != 1 )
        parent = &processes[pidParent];
    else
        parent = NULL;
    */

    pcb_pointer process = memalloc(sizeof(pcb));
    processes[pid-1] = process;

    // setting the pcb of the new process
    process->name=argv[0];
    process->pid = pid;
    process->alive = 1;
    process->fd[STDIN] = STDIN;
    process->fd[STDOUT] = STDOUT;
    process->fd[STDERR] = STDERR;
    process->foreground = foreground;
    process->parent = pidParent;

    // adding scheduling info
    process->scheduling_info = memalloc(sizeof(struct sch_info));

    process->scheduling_info->p_state = READY;
    process->scheduling_info->CPU_time = 0;
    if ( process->pid == 1 )
        process->scheduling_info->priority = ALWAYSACTIVE;
    else
        process->scheduling_info->priority = MOSTIMP;
    

    
    if (pid != 1){
        pcb_pointer parent = processes[pidParent];
        p_children child = memalloc( sizeof(struct children) );
        child->pid = pid;
        child->next = NULL;
        if ( parent->childs.first == NULL ){
            parent->childs.first = child;
            parent->childs.last = child;
        }else{
            struct children * current = parent->childs.first;
            while ( current->next != NULL ){
                current = current->next;
            }
            current->next = child;
            parent->childs.last = child;
        }
    }else {
        process->childs.first = NULL;
        process->childs.last = NULL;  
    }
                                                    // starts from behind
    uint8_t* stack_end = memalloc(STACK_MEM);      // Saving space for the process' stack
    if ( stack_end == NULL )
        return -1; 
    uint8_t* stack_start = stack_end + STACK_MEM;  // Going to the start of the stack

    process->stack_start = stack_start;
    process->stack_end = stack_end;
    process->stack_current = prepare_process(stack_start, rip, argc, argv);          // At last, we asign the RSP to the registers

    add_process_to_scheduling(process->pid, processes[(process->pid)-1]->scheduling_info, process->stack_current);
    pid++;
    _sti();
    forceTimerTick();
    
    return process->pid;
}

void change_rsp_process( int pid, uint8_t* rsp ){
    processes[pid]->stack_current = rsp;
}


// after calling this function must be made a tick interruption
int set_status( int _pid, int newState){

    _cli();

    if ( _pid > pid || _pid < 2 || _pid == 1  )
        return -1;
    pcb_pointer process = processes[_pid-1];
    if ( process->alive ){
        if ( newState == process->scheduling_info->p_state )
            return newState;
        if ( newState == BLOCKED ){
            process->scheduling_info->p_state = BLOCKED;
            int result = scheduling_to_blocked(_pid);
            if ( result )
                return -1;
            
        }else if ( process->scheduling_info->p_state == BLOCKED ){
            process->scheduling_info->p_state = newState;
            process->scheduling_info->priority = MOSTIMP;
            int result = blocked_to_scheduling(_pid);
            if ( result )
                return -1;
        }
        return newState;
    }

    _sti();

    return-1;

}

int set_fd( int _pid, int new_fd, char pos ){
    
    if ( pos < 0 || pos > 2 || _pid > pid || _pid < 2)
        return ERROR;
    processes[_pid]->fd[pos] = new_fd;
    return SUCCESS;
}


void show_processes(){

    _cli();

    print("PID    NAME    STACK    BASEPR    FOREGROUND");
    for( int i=0; i < pid-1; i++ ){
        pcb_pointer process = processes[i];

        // futuramente imprimir stdin y stdout
        enter();
        printDec(process->pid);
        tab();
        print(process->name);
        tab();
        printHex(process->stack_current);
        tab();
        printHex(process->stack_start);
        tab();
        printDec(process->foreground);
        enter();
    }

    _sti();

}

uint8_t* getSchedulingInfo(int pid){
    return processes[pid]->scheduling_info;
}


int get_pid_parent(){
    int _pid = get_pid();
    pcb_pointer process = processes[_pid];
    if ( process->alive ){
        return process->parent;
    }
    return -1;
}

// returns 0 if eliminated correctly, 1 if error and -1 if not found
int kill_process(int _pid){
    if ( _pid < 1 || _pid > pid )
        return 1;
    free(processes[_pid]->stack_end);
    processes[_pid]->alive = 0;
    return delete_process_scheduling(_pid);
}

void ending_free(){
    for ( int i=0; i < pid; i++ ){
        pcb_pointer process = processes[i];
        if ( process->alive ){
            free(process->stack_end);
            free( process->scheduling_info );
            // liberar childs
        }
    }
}
