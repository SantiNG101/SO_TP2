
#include <process.h>

// chequeo en cada timer si se paso del fin del stack para verificar que no haya stack overflow


struct children_info{
    struct childern_info* first;
    struct childern_info* current;
};

struct children{
    int pid;
};

typedef struct pcb {
    char* name;
    int pid;                    // no tan necesario en el momento de hacer un array de procesos, si es necesario si se hace una lista
    int stdin;
    int stdout;
    int stderr;
    struct sch_info scheduling_info;
    struct children_info childs;
    uint8_t* stack_current;
    uint8_t* stack_end;             // where memory ends
    uint8_t* stack_start;           // where memory starts
    pcb_pointer parent;             // pointer to the information of the parent
    int alive;                               
    int active;
    // array of fd and devices
}pcb;

static int pid =1;
static pcb processes[MAX_PROCESSES_SUPPORTED];

// allocs initial memory structure and creates the init process
void process_init(){

    ncPrint("Initializing process init");
    ncNewline();
    char* argv[] = {"init"};
    // initialiaze scheduler
    initialize_scheduler();
    // Creation of process init.
    int hola = process_create(0x0, 0x400000, 1 ,argv );          // rip = start text segment of the userland          
    ncPrintDec(hola);

}

// for syscall fork => return;
int process_create( int pidParent, uint8_t* rip, int argc, char* argv[] ){

    pcb_pointer parent;
    if ( pid != 1 )
        parent = &processes[pidParent];
    else
        parent = NULL;

    pcb_pointer process = &processes[pid-1];

    // setting the pcb of the new process
    process->name=argv[0];
    process->pid = pid;
    processes->parent = parent;
    process->active = 1;            // not active = zombie
    process->alive = 1;
    process->stdin = STDIN;
    process->stdout = STDOUT;
    process->stderr = STDERR;
    
    process->parent = parent;
    process->scheduling_info.p_state = READY;
    if ( process->pid == 1 )
        process->scheduling_info.priority = ALWAYSACTIVE;
    else
        process->scheduling_info.priority = MOSTIMP;
    process->scheduling_info.CPU_time = 0;

    if (pid != 1){
        struct children child;
        child.pid = pid;
        if ( parent->childs.first == NULL ){
            parent->childs.first = &child;
        }
        parent->childs.current = &child;
    }

    process->childs.first = NULL;
    process->childs.current = NULL;                    
    
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
    forceTimerTick();
    
    return process->pid;
}

void change_rsp_process( int pid, uint8_t* rsp ){
    processes[pid].stack_current = rsp;
}


// after calling this function must be made a tick interruption
int set_status( int _pid, int newState){

    if ( _pid > pid || _pid < 2 || _pid == 1  )
        return -1;
    pcb process = processes[_pid];
    if ( process.alive ){
        if ( newState == process.scheduling_info.p_state )
            return newState;
        if ( newState == BLOCKED ){
            process.scheduling_info.p_state = BLOCKED;
            int result = scheduling_to_blocked();
            if ( result )
                return -1;
            
        }else if ( process.scheduling_info.p_state == BLOCKED ){
            process.scheduling_info.p_state = newState;
            process.scheduling_info.priority = MOSTIMP;
            blocked_to_scheduling(_pid);
        }
        return newState;
    }

    return -1;

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
        return process.parent->pid;
    }
    return -1;
}
