#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <mm.h>
#include <process.h>

#define MAX_PIPE_LENGTH 1024*2  // 2k
#define MAX_PIPES 1000
#define READ 0
#define WRITE 1
#define EOF -1

typedef struct pipe* p_pipe;

long create_pipe();
// if given an id in 0, must be created
// returns the pipe identifier, if error return an unexistent id 0
uint32_t open_pipe_for_pid( uint32_t pid, uint32_t id, uint8_t mode );
void close_pipe_for_pid( uint32_t pid, uint32_t id, uint8_t mode );
void free_pipe( long index, p_pipe pipe );
void move_data( int count, p_pipe pipe );
long read_pipe( uint32_t id, char* destination_buffer, uint32_t length );
int write_pipe( uint32_t id, char* source_buffer, uint32_t length );



#endif