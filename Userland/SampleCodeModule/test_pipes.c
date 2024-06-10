#include <stdint.h>
#include "libc/include/stdio.h"


int process_one( int argc, char* argv[] ){
    int fd = get_fd(1);
    while( fd == 1 ){
        yield();
        fd = get_fd(1);
    }

    printf( "hola mundo" );

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

    sscanf(buff, "%s");

    printf(buff);

    pipe_close(getpid(), fd, 0);
    exit_process(0);
    return 0;

}

int64_t test_pipes( int argv, char* argc[] ) {
    

    char* argv1[] = {"./process_one"};
    int pid_1 = execve(getpid(), process_one, 1, argv1, 0);


    char* argv2[] = {"./process_two"};
    int pid_2 = execve(getpid(), process_two, 1, argv2, 1);

    uint32_t pipe = pipe_open(pid_1, 0, 1);
    pipe = pipe_open(pid_2, pipe, 0);

    exit_process(0);

}


