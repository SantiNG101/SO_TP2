#include <stdint.h>
#include <string.h>
#include "include/sharedMem.h"

#define MAX_NAME_LENGTH 20
#define MAX_SHARED_MEMS 20

struct shared_mem {
    char name[MAX_NAME_LENGTH];
    void *ptr;
};

typedef struct shared_mem *shared_mem_ptr;
shared_mem_ptr shared_mems[MAX_SHARED_MEMS];
int last_shared_mem_created = -1;

// Function to get shared memory by name
shared_mem_ptr get_shared_mem_by_name(char *name) {
    for (int i = 0; i < MAX_SHARED_MEMS; i++) {
        if (shared_mems[i] != NULL && strcmp(name, shared_mems[i]->name) == 0) {
            return shared_mems[i];
        }
    }
    return NULL;
}

// Function to open or create shared memory
void *open_shared_mem(char *name, int size) {
    shared_mem_ptr new_shared_mem = get_shared_mem_by_name(name);
    if (new_shared_mem == NULL) {
        int i;
        for (i = last_shared_mem_created + 1; i != last_shared_mem_created; i++) {
            if (i == MAX_SHARED_MEMS) {
                i = 0;
                if (i == last_shared_mem_created) break;
            }
            if (shared_mems[i] == NULL) break;
        }

        // If no space is available
        if (i == last_shared_mem_created && shared_mems[i] != NULL) {
            return NULL;
        }

        last_shared_mem_created = i;
        new_shared_mem = myMalloc(sizeof(struct shared_mem));
        new_shared_mem->ptr = myMalloc(size);
        memcpy(new_shared_mem->name, name, MAX_NAME_LENGTH - 1);
        new_shared_mem->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        shared_mems[i] = new_shared_mem;
    }
    return new_shared_mem->ptr;
}

