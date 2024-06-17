#include <stdint.h>
#include <stdio.h>
#include "syscall.h"
#include "test_util.h"
#include "libc/include/stdio.h"

#define MINOR_WAIT 10 
#define WAIT 20      

#define TOTAL_PROCESSES 3
#define LOWEST 2  
#define MEDIUM 1  
#define HIGHEST 0 

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

int64_t test_prio(uint64_t argc, char *argv[]) {
  int64_t pids[TOTAL_PROCESSES];
  int pid = getpid();
  char *args[] = {"endless_loop_print"};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = execve(pid,endless_loop_print,1, args,1);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    lower_prio(pids[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    lower_prio(pids[i]);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblock(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill_process(pids[i]);

  exit_process(0);
}
