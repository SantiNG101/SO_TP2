#include <stdint.h>
#include "libc/include/stdio.h"
#include "libc/include/stdlib.h"


int process_one( int argc, char* argv[] ){
    int fd = get_fd(1);
    while( fd == 1 ){
        yield();
        fd = get_fd(1);
    }

    printf( "hol" );

    pipe_close(getpid(), fd, 1);
    exit_process(0);
    return 0;
}

int process_two( int argc, char* argv[] ){

    int fd = get_fd(0);
    while( fd == 0 ){
        yield();
        fd = get_fd(0);
    }

    char buff[200] = {0};
    char c;
    int end=0;

    for( int i=0; i < 200 && !end; i++ ){
        c = getChar();
        if ( c == -1 ){
            end = 1;
            break;
        }
        buff[i] = c;
    }

    printf(buff);
    pipe_close(getpid(), fd, 0);

    exit_process(0);
    return 0;

}

int64_t test_pipes( int argv, char* argc[] ) {
    
    int pid = getpid();

    char* argv1[] = {"./process_one"};
    int pid_1 = execve(pid, process_one, 1, argv1, 0);


    char* argv2[] = {"./process_two"};
    int pid_2 = execve(pid, process_two, 1, argv2, 0);

    uint32_t pipe = pipe_open(pid_1, 0, 1);
    set_fd(pid_1, pipe, 1);

    pipe = pipe_open(pid_2, pipe, 0);
    set_fd(pid_2, pipe, 0);

    wait_children(pid);

    exit_process(0);

}


