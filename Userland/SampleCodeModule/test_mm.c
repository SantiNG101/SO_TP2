#include "test_util.h"
#include "stdio.h"
#include <strings.h>
#include "tests.h"

#define MAX_BLOCKS 128

typedef struct MM_rq
{
    void *address;
    uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[])
{
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory;

    if (argc != 2)
    {
        exit_process(-1);
        return -1;
    }

    if ((max_memory = satoi(argv[1])) <= 0)
    {
        exit_process(-1);
        return -1;
    }

    max_memory = max_memory << 20;

    int i = 0;

    while (1)
    {
        rq = 0;
        total = 0;

        printf("----------Begin----------\n");
        printf("Allocated blocks: %d\n", rq);
        printf("Total memory: %d\n", total);
        printf("Max memory: %d\n", max_memory);

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory)
        {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;

            mm_rqs[rq].address = alloc(mm_rqs[rq].size);

            if (mm_rqs[rq].address)
            {
                total += mm_rqs[rq].size;
                printf("--------------------\n");
                printf("Size: %d\n", mm_rqs[rq].size);
                printf("Allocating block: %d\n", rq);
                printf("Total memory: %d\n", total);
                printf("Max memory: %d\n", max_memory);
                rq++;
            }
            else
            {
                printf("Failed to allocate memory block of size: %d\n", mm_rqs[rq].size);
                break;
            }
        }

        printf("--------------------\n");
        printf("Finished allocating\n");

        // Set
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                printf("Begin setting for block %d of size %d\n", i, mm_rqs[i].size);
                memset(mm_rqs[i].address, i, mm_rqs[i].size);
            }
        }
        printf("Set OK\n");

        // Check
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                {
                    printf("test_mm ERROR\n");
                    exit_process(-1);
                    return -1;
                }
            }
        }
        printf("Check OK\n");

        // Free
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
                free_alloc(mm_rqs[i].address);
        }

        printf("Free OK\n");
        printf("----------End----------\n");
    }
}
