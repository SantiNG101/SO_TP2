
#include "tests.h"



enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int unblock( int pid){
    return set_status(pid, 1);
}

int block( int pid){
    return set_status(pid, 0);
}



int64_t test_processes(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {"endless_loop"};

  if (argc != 2){
    exit_process(-1);
    return -1;
  }
  if ((max_processes = satoi(argv[1])) <= 0)
    exit_process(-1);

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = execve(getpid(),(Function)endless_loop, 1, argvAux, 1);

      if (p_rqs[rq].pid == -1) {
        printf("test_processes: ERROR creating process\n");
        exit_process(-1);
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (kill_process(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR killing process\n");
                exit_process(-1);
                return -1;
              }
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (block(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR blocking process\n");
                exit_process(-1);
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (unblock(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            exit_process(-1);
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
    printf("test_processes: All processes have been killed\n");
    exit_process(0);
  }
}


void test_processes_wrapper(int argc, char* argv[]){
    char * newargv[] = {"test_processes", "3"};
    execve(getpid(),test_processes, 2, newargv, 1);
}