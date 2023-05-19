#include "include/lib.h"

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}




//INTENTO DE ALOCAR MEMORIA ESTATICA DE MANERA DINAMICA (Para un manipular mas facilmente los strings)
#define MEMORY_SIZE 1024            // Tamano del array donde se asigna la memoria le reservo 1kb
static char memory[MEMORY_SIZE];    // Dicho array

/*
 * Struct -> Define bloques que tienen como dato:
 * El espacio que ocupan.
 * Si esta siendo utilizado.
 * Un puntero al siguiente en la lista.
 */

typedef struct BlockHeader {
    size_t size;
    int is_free;
    struct BlockHeader* next;
} BlockHeader;

//Puntero estatico al comienzo del bloque de memoria
static BlockHeader* first_block;

void* myMalloc(size_t size) {
    static int initialized = 0;

    if (!initialized) {
        // Inicializo la cabecera del bloque haciendo que apunte al comienzo del array memory
        first_block = (BlockHeader*)memory;
        first_block->size = MEMORY_SIZE - sizeof(BlockHeader);
        first_block->is_free = 1;
        first_block->next = NULL;
        initialized = 1;
    }

    // Ahora la idea es buscar un bloque con suficiente espacio
    BlockHeader* curr_block = first_block;
    while (curr_block) {
        if (curr_block->is_free && curr_block->size >= size) {

            // Si el bloque es mas grande de lo necesario, reservo el espacio y dejo el resto del espacio libre;
            if (curr_block->size >= size + sizeof(BlockHeader)) {

                BlockHeader* new_block = (BlockHeader*)((char*)curr_block + sizeof(BlockHeader) + size);    //puntero al nuevo bloque va a estar en la posicion curr_block + el size del struct + lo que quiero reservar de memoria

                    //new_block reduce su size en size y sizeof(BlockHeader), esta libre, y su next es el next del current block
                new_block->size = curr_block->size - size - sizeof(BlockHeader);
                new_block->is_free = 1;
                new_block->next = curr_block->next;


                    //current block toma el size pedido, se reserva y el siguiente es el new block
                curr_block->size = size;
                curr_block->is_free = 0;
                curr_block->next = new_block;

            }
            else {  //tiene el espacio justo para este bloque => lo reservo
                curr_block->is_free = 0;
            }

            // Retorno un puntero a esta seccion del bloque
            return (void*)((char*)curr_block + sizeof(BlockHeader));
        }
            //si no hay espacio sigo al siguiente
        curr_block = curr_block->next;
    }

    // No hay bloques de memoria disponibles con suficiente espacio
    return NULL;
}

void myFree(void* ptr) {
    if (ptr == NULL) {
        return;  // Si apunta a nada => no hay nada que liberar
    }

    // Obtengo el puntero al bloque que quiero liberar y cambio el flag is_free a 1
    BlockHeader* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    block->is_free = 1;

    // Fusion de los bloques consecutivos que esten libres
    BlockHeader* curr_block = first_block;    //recorro desde el principio
    while (curr_block) {
        if (curr_block->is_free) {
            if (curr_block->next && curr_block->next->is_free) {
                curr_block->size += sizeof(BlockHeader) + curr_block->next->size;
                curr_block->next = curr_block->next->next;
            }
        }
        //paso al siguiente
        curr_block = curr_block->next;
    }
}

//#-------------------------------------------------------#