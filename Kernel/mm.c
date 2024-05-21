#include <mm.h>
#include <stddef.h>

/*
typedef struct mem_block{
    char* start;
    char* end;
    char used;
}block;

void* memalloc(uint64_t size);
void free(void* mem);

char memarray[521142272] = {0};
*/

typedef struct memManagerCDT {
    char * start;
    char * end;
    char * current_address;
    int chunk_size;
    memChunkADT chunks[MAX_CHUNKS]; // NOT a freelist, it's a list of all possible chunks
    int current_chunk;
} memManagerCDT;

typedef struct memManagerCDT * memManagerADT;

typedef struct memChunkCDT {
    int used;
    char * starts_at;
    int length;
} memChunkCDT;

typedef struct memChunkADT * memChunkADT;

memManagerADT mem_initialize(int chunk_size) {
    if (chunk_size < 1 || (MEM_END - MEM_START) < chunk_size)
        return ERROR;

    memManagerADT mem_manager = (memManagerADT) MEM_START;

    mem_manager->start = MEM_START;
    mem_manager->end = MEM_END;
    mem_manager->chunk_size = chunk_size;

    // initialize all posible chunks
    for (int i = 0; i < MAX_CHUNKS; i++) {
        memChunkADT new_chunk = (memChunkADT) (MEM_START + sizeof(memManager) + (i * chunk_size));
        new_chunk->used = FALSE;
        new_chunk->starts_at = MEM_START + sizeof(memManager) + (i * chunk_size);
        new_chunk->length = chunk_size;
        
        mem_manager->chunks[i] = new_chunk;
    }

    mem_manager->current_address = MEM_START + sizeof(memManager) + sizeof(memChunk) * MAX_CHUNKS;

    // first chunk -> memory manager with the list of chunks
    mem_manager->chunks[0]->used = TRUE;
    mem_manager->chunks[0]->starts_at = MEM_START;
    mem_manager->chunks[0]->length = sizeof(memManager) + sizeof(memChunk) * MAX_CHUNKS;

    mem_manager->current_chunk = 1;

    return mem_manager;    
}

void * mem_alloc(memManagerADT mem_manager, int size) {
    if (mem_manager->current == MEM_END || (mem_manager->current + size) > MEM_END)
        return ERROR;

    char * direction = mem_manager->current_address;

    mem_manager->current += size; // TODO: calculate log size in buddy here

    int chunks_needed = size / mem_manager->chunk_size;
    if (size % mem_manager->chunk_size != 0)
        chunks_needed++;

    // TODO: search for the first chunk that fits
    while (int i = 0; i < MAX_CHUNKS; i++) {

        if (mem_manager->chunks[i]->used == TRUE)
            continue;

        int j = 0;
        while (j < chunks_needed) {
            if (mem_manager->chunks[i+j]->used == TRUE)
                break;
            j++;
        }

        if (j == chunks_needed)
            // found a chunk that fits from [i, i+j]

            direction = mem_manager->chunks[i]->starts_at;
            
            for (int k = i; k < chunks_needed; k++) {
                mem_manager->chunks[i+k]->used = TRUE;
                mem_manager->chunks[i+k]->length = size;
            }
    }

    mem_manager->current_chunk++; // not used
    mem_manager->current_address += size;

    return direction;
}

void * free(memManagerADT mem_manager, void * allocated_memory) {

    if (allocated_memory == NULL || allocated_memory < MEM_START || allocated_memory > MEM_END)
        return ERROR;

    for (int i = 0; i < MAX_CHUNKS: i++) {
        if (mem_manager->chunks[i]->starts_at == allocated_memory) {
            int chunk_length = mem_manager->chunks[i]->length;
            for (int j = 0; j < chunk_length; j++) {
                mem_manager->chunks[i+j]->used = FALSE;
                mem_manager->chunks[i+j]->length = 0;
            }
            return;
        }
    }
    return ERROR;
}