#ifndef BITMAP

#include <BitmapMemoryManagerADT.h>

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

typedef struct memManagerCDT
{
    char *start;
    char *end;
    int chunk_size;
    int occupied;
    memChunkADT chunks[MAX_CHUNKS]; // list of all possible chunks
} memManagerCDT;

typedef struct memManagerCDT *memManagerADT;

typedef struct memChunkCDT
{
    int used;
    char *starts_at;
    int length; // length seria la longitud total de la direccion memoria pedida a la que pertenece el chunk, me sirve para iterar en el free
} memChunkCDT;

typedef struct memChunkCDT *memChunkADT;

memManagerADT mem_initialize()
{
    if (CHUNK_SIZE < 1 || (MEM_END - MEM_START) < CHUNK_SIZE)
        return ERROR;

    memManagerADT mem_manager = (memManagerADT)MEM_START;

    mem_manager->start = MEM_START;
    mem_manager->end = MEM_END;
    mem_manager->chunk_size = CHUNK_SIZE;
    mem_manager->occupied = 0;

    // initialize all posible chunks
    for (int i = 0; i < MAX_CHUNKS; i++)
    {
        memChunkADT new_chunk = (memChunkADT)(MEM_START + sizeof(memManager) + (i * CHUNK_SIZE));
        new_chunk->used = FALSE;
        new_chunk->starts_at = MEM_START + sizeof(memManager) + (i * CHUNK_SIZE) + 1;
        new_chunk->length = 0;

        mem_manager->chunks[i] = new_chunk;
    }

    // first chunks -> memory manager with the list of chunks
    int chunks_needed = (sizeof(memManagerCDT) + sizeof(memChunkCDT) * MAX_CHUNKS) / CHUNK_SIZE;
    for (int i = 0; i < chunks_needed; i++)
    {
        mem_manager->chunks[i]->used = TRUE;
        mem_manager->chunks[i]->length = sizeof(memManagerCDT) + sizeof(memChunkCDT) * MAX_CHUNKS;
    }
    return mem_manager;
}

void *mem_alloc(memManagerADT mem_manager, int size)
{
    if (mem_manager->occupied == MAX_CHUNKS || size < 1 || size > (MEM_END - MEM_START))
        return ERROR;

    int chunks_needed = size / mem_manager->chunk_size;
    if (size % mem_manager->chunk_size != 0)
        chunks_needed++;
    
    if (mem_manager->occupied + chunks_needed > MAX_CHUNKS)
        return ERROR;

    mem_manager->occupied += chunks_needed;

    char * direction;

    while (int i = 0; i < MAX_CHUNKS; i++)
    {

        if (mem_manager->chunks[i]->used == TRUE)
            continue;

        int j = 0;
        while (j < chunks_needed)
        {
            if (mem_manager->chunks[i + j]->used == TRUE)
                break;
            j++;
        }

        if (j == chunks_needed)
            // found a chunk that fits from [i, i+j)
            direction = mem_manager->chunks[i]->starts_at;

        for (int k = i; k < chunks_needed; k++)
        {
            mem_manager->chunks[i + k]->used = TRUE;
            mem_manager->chunks[i + k]->length = size;
        }
    }
    return direction;
}

void *free(memManagerADT mem_manager, void *allocated_memory)
{

    if (allocated_memory == NULL || allocated_memory < MEM_START || allocated_memory > MEM_END)
        return ERROR;

    for (int i = 0; i < MAX_CHUNKS : i++)
    {
        if (mem_manager->chunks[i]->starts_at == allocated_memory)
        {
            int chunk_length = mem_manager->chunks[i]->length;
            for (int j = 0; j < chunk_length; j++)
            {
                mem_manager->chunks[i + j]->used = FALSE;
                mem_manager->chunks[i + j]->length = 0;
            }
            return;
        }
    }
    return ERROR;
}

#endif