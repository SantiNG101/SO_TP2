
#include <process.h>


struct sch_info{
    int p_state;
    int priority;
    uint64_t* pqueue;                //pointer to scheduling queue
    uint16_t CPU_time;
                                    // posible other characteristics that are going to be used by the scheduler to determine its priority
};

struct children_info{
    struct childern_info* first;
    struct childern_info* current;
};

struct children{
    int pid;
};

typedef struct pcb {
    int pid;                    // no tan necesario en el momento de hacer un array de procesos, si es necesario si se hace una lista
    //uint64_t registers[17];         // 16 registers + RIP (in last position)
    struct sch_info scheduling_info;
    struct children_info childs;
    uint64_t stack_current;
    uint64_t stack_end;             // where memory ends
    uint64_t stack_start;           // where memory starts
    pcb_pointer parent;             // pointer to the information of the parent
                                    // array of fd and devices
}pcb;

static int pid =1;
static pcb processes[1000];

// allocs initial memory structure and creates the init process
void process_init(){

    char* argv[] = {"init"};
    // Creation of process init.
    process_create(0x0, 0x4000, 1 ,argv );          // rip = start text segment of the userland          

}

// for syscall fork => return;
void process_create( int pidParent, uint64_t rip, int argc, char* argv[] ){

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
    uint64_t* stack_end = memalloc(STACK_MEM);      // Saving space for the process' stack
    uint64_t* stack_start = stack_end + STACK_MEM;  // Going to the start of the stack

    stack_start = align_stack(stack_start);         // Aligning stack
    uint64_t* stack_current = stack_start;          // Setting RSP
    stack_current -=sizeof(uint64_t);               // Setting usable stack

    (*stack_current) = 0x0;                         // Loading the SS
    stack_current -=sizeof(uint64_t);

    (*stack_current) = stack_current;               // Loading the RSP 
    stack_current -=sizeof(uint64_t);

    (*stack_current) = 0x202;                       // Loading RFLAGS
    stack_current -=sizeof(uint64_t);

    (*stack_current) = 0x8;                         // Loading CS
    stack_current -=sizeof(uint64_t);

    (*stack_current) = rip;                         // Loading the RIP
    stack_current -=sizeof(uint64_t);

    uint64_t* regs = getRegisters();
    regs[5] = argc;
    regs[6] = argv;
    
    for ( int i=0; i< 16; i++ ){
        (*stack_current) = regs[i];                 // saving first RAX and last RSP
        stack_current -=sizeof(uint64_t);       
    }

    stack_current +=sizeof(uint64_t); 
    (*stack_current) = stack_current;              // Loading the registers, un for seguramente            


    process->stack_start = stack_start;
    process->stack_end = stack_end;
    process->stack_current = stack_current;          // At last, we asign the RSP to the registers

    return;
}


uint64_t* align_stack( uint64_t* init){
    while( ((int)init%8) != 0 ){
        init= init-1;
    }
        
    return init;
}


