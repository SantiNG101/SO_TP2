
#include <pipes.h>


struct pipe {
    char buffer[MAX_PIPE_LENGTH];
    uint32_t id;
    uint8_t inpid;
    uint8_t outpid;
    uint32_t current_size;       // to know how much info is in the pipe
    uint8_t is_blocking;
    semaphore_ptr sem;

};



static p_pipe pipes[MAX_PIPES];
static uint32_t pipe_id = 3;
static uint32_t pipe_count = 0;


long get_next_free_index(){
    for ( int i=0; i < MAX_PIPES; i++ ){
        p_pipe aux = pipes[i];
        if ( aux == NULL )
            return i;
    }
    return -1;
}

long get_pipe_from_id( uint32_t id ){
    for ( int i=0; i < MAX_PIPES; i++ ){
        p_pipe aux = pipes[i];
        if ( aux != NULL && aux->id == id )
            return i;
    }
    return -1;
}

int intLength(int64_t number) {
    int length = 0;
    if (number == 0) return 1; // caso especial para el número 0
    if (number < 0) {
        length++; // espacio para el signo negativo
        number = -number;
    }
    while (number != 0) {
        length++;
        number /= 10;
    }
    return length;
}

// Función para convertir un entero a una cadena
void intToString(int64_t number, char *buffer) {
    int isNegative = 0;
    int length = intLength(number);

    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    buffer[length] = '\0'; // Terminar la cadena con el carácter nulo
    length--; // Ajustar para índice del array

    while (length >= 0) {
        buffer[length] = (number % 10) + '0'; // Convertir el dígito a carácter
        number /= 10;
        length--;
    }

    if (isNegative) {
        buffer[0] = '-'; // Añadir el signo negativo al principio
    }
}



long create_pipe(){
    p_pipe pipe = memalloc(sizeof(struct pipe));
    if ( pipe == NULL )
        return -1;

    long index = get_next_free_index();
    if ( index == -1 )
        return -1;
    pipes[index] = pipe;
    pipe->id = pipe_id++;
    pipe->current_size = 0;
    pipe->inpid = 0;
    pipe->outpid = 0;
    pipe->is_blocking = 0;

    char buff[20];
    intToString(pipe->id, buff);
    pipe->sem=create_semaphore(buff, 1);

    pipe_count++;


    return index;
}

// if given an id in 0, must be created
// returns the pipe identifier, if error return an unexistent id 0
uint32_t open_pipe_for_pid( uint32_t pid, uint32_t id, uint8_t mode ){

    if ( (mode != READ && mode != WRITE))
        return 0;
    
    long index = get_pipe_from_id(id);
    if ( index == -1 ){
        index = create_pipe();
        if ( index == -1 )
            return 0;
    }

    p_pipe pipe = pipes[index];

    if ( mode == WRITE ){
        pipe->inpid = pid;
    }else {
        pipe->outpid = pid;
    }

    return pipe->id;
}

void close_pipe_for_pid( uint32_t pid, uint32_t id, uint8_t mode ){
    
    if ( (mode != READ && mode != WRITE) || id < 3 || id > MAX_PIPES )
        return;

    long index = get_pipe_from_id(id);
    if ( index == -1 )
        return;
    
    p_pipe pipe = pipes[index];

    if ( pipe->outpid == pid ){
        free_pipe( index, pipe );
    }else if ( pipe->inpid == pid ){
        pipe->inpid = 0;
        
        while ( pipe->current_size >= MAX_PIPE_LENGTH ){
            set_status(pid, BLOCKED);
            yield();
        }
		pipe->buffer[pipe->current_size++] = -1;

        if ( pipe->is_blocking){
            set_status(pipe->outpid, READY);
            pipe->is_blocking=0;
        }

    }
    return;
}

void free_pipe( long index, p_pipe pipe ){
    free(pipe);
    pipes[index] = NULL;
    pipe_count--;
    return;
}

void move_data( int count, p_pipe pipe ){
    for ( int i=0, j=count; pipe->current_size > i ;  i++,j++ ){
        pipe->buffer[i] = pipe->buffer[j];
    }
    return;
}


long read_pipe( uint32_t id, char* destination_buffer, uint32_t length ){

    long index = get_pipe_from_id(id);
    if ( index == -1 ){
        return -1;
    }

    p_pipe pipe = pipes[index];
    if ( pipe->outpid != get_pid() ){
        return -1;
    }

    char eof_read = 0;
    uint32_t read_bytes = 0;

    while ( read_bytes < length && !eof_read ){

        // block if no data avaliable
        if ( pipe->current_size == 0 && pipe->buffer[0] != EOF ){
            pipe->is_blocking = 1;
            set_status( pipe->outpid, BLOCKED );
            yield();
        }
        // wait sem
        semaphore_wait(pipe->sem);

        while ( (pipe->current_size > 0 || pipe->buffer[read_bytes] == EOF) && read_bytes < length){
            destination_buffer[read_bytes] = pipe->buffer[read_bytes];
            if( destination_buffer[read_bytes++] == EOF ){
                eof_read = 1;
                break;
            }
            pipe->current_size--;
        }
        if ( read_bytes > 0 )
            move_data(read_bytes, pipe);

        //
        semaphore_post(pipe->sem);
        
        // unblock if it was full
        if ( pipe->is_blocking ){
            set_status(pipe->inpid, READY);
            pipe->is_blocking = 0;
        }
        
    }

    return read_bytes;
}

int write_pipe( uint32_t id, char* source_buffer, uint32_t length ){
    long index = get_pipe_from_id(id);
    if ( index == -1 ){
        return -1;
    }

    p_pipe pipe = pipes[index];
    if ( pipe->inpid != get_pid() ){
        return -1;
    }

    char eof_write = 0;
    uint32_t write_bytes = 0;

    while( write_bytes < length && !eof_write ){
        if ( pipe->current_size >= MAX_PIPE_LENGTH ){
            pipe->is_blocking = 1;
            set_status( pipe->inpid, BLOCKED );
            yield();
        }
        //wait sem
        semaphore_wait(pipe->sem);

        while( pipe->current_size < MAX_PIPE_LENGTH && write_bytes < length ){
            pipe->buffer[pipe->current_size] = source_buffer[write_bytes++];
            if ( pipe->buffer[pipe->current_size++] == EOF ){
                eof_write = 1;
                break;
            }
        }
        // post sem
        semaphore_post(pipe->sem);

        if ( pipe->is_blocking){
            set_status(pipe->outpid, READY);
            pipe->is_blocking=0;
        }
    }
    return write_bytes;
}

