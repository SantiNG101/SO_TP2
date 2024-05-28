
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
    int pid;                    // no tan necesario en el momento de hacer un array de procesos, si es necesario si se hace una lista
    int stdin;
    int stdout;
    struct sch_info scheduling_info;
    struct children_info childs;
    uint8_t stack_current;
    uint8_t stack_end;             // where memory ends
    uint8_t stack_start;           // where memory starts
    pcb_pointer parent;             // pointer to the information of the parent
                                    // array of fd and devices
}pcb;

static int pid =1;
static pcb processes[1000];

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
int process_create( int pidParent, uint8_t rip, int argc, char* argv[] ){

    pcb_pointer parent;
    if ( pid != 1 )
        parent = &processes[pidParent];
    else
        parent = NULL;

    pcb_pointer process = &processes[pid-1];

    // setting the pcb of the new process
    process->pid = pid++;
    processes->parent = parent;
    
    process->parent = parent;
    process->scheduling_info.p_state = READY;
    if ( pid == 1 )
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
    uint8_t* stack_start = stack_end + STACK_MEM;  // Going to the start of the stack

    process->stack_start = stack_start;
    process->stack_end = stack_end;
    process->stack_current = prepare_process(stack_start, rip, argc, argv);          // At last, we asign the RSP to the registers

    addProcessToScheduling(pid, &processes[pid-1].scheduling_info,process->stack_current);

    return pid;
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
