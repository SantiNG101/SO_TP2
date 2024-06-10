
#include "libc/include/stdio.h"
#include "libc/include/stdlib.h"

#define MAX_PHILOSOPHERS 7
#define MIN_PHILOSOPHERS 3
#define INIT_PHILOSOPHERS 5
#define HUNGRY 0
#define EATING 1
#define THINKING 2
#define NULL ((void *)0)
#define BACK 0
#define FORE 1

#define MUTEX "phyloMutex"

int64_t initPhyloReunion(int argc, char* argv[]);


char philosophersName [MAX_PHILOSOPHERS][24]={"Aristoteles","Platon","Socrates","Descartes","Kant","Nietzsche","Sartre","Russell","Machiavelli"};

void getInput();
void addPhilo(int i);
void test(int philoNum);
void takeForks(int philoNum);
void putForks(int philoNum);
void eat(int philoNumber);
void think(int philoNumber);

typedef struct Philosopher {
    char name[24];
    int state;
    int pid;
    int64_t sem;
}Philosopher;

static int philoAmount = 0;
typedef struct Philosopher* philo;

static philo philosophers[MAX_PHILOSOPHERS];
static int64_t sem = 0;


void philosopherActivity(int argc, char* argv[]) {
    semaphore_wait(MUTEX);
    int philoNumber=philoAmount;
    philoAmount++;
    semaphore_post(MUTEX);
    while(1) {
        think(philoNumber);
        takeForks(philoNumber);
        eat(philoNumber);
        putForks(philoNumber);
    }
}

int64_t initPhyloReunion(int argc, char* argv[]) {
    sem = create_semaphore(MUTEX, 1);
    philoAmount=0;

    semaphore_wait(MUTEX);
    for(int i=0;i<INIT_PHILOSOPHERS;i++) {
        addPhilo(i);
    }
    semaphore_post(MUTEX);
    while(1) {
        getInput();
        wait(500);
        semaphore_wait(MUTEX);
        for(int i=0;i<philoAmount;i++) {
            if(philosophers[i]->state == EATING) {
                printFormat("E");
            }
            else {
                printFormat(".");
            }
        }
        printFormat("\n");
        postSem(MUTEX);
    }
}

void addPhilo(int i) {
    philosophers[i] = memalloc(sizeof(Philosopher));
    if(philosophers[i]==NULL) {
        printFormat("Error creating Philosopher %s\n",philosophersName[i]);
        return -1;
    }
    strcpy(philosophers[i]->name,philosophersName[i]);
    philosophers[i]->state = THINKING;
    philosophers[i]->sem = create_semaphore(philosophersName[i], 1);
    char* argv[] = {philosophersName[i]};
    int pid= execve( getpid(),philosopherActivity,1,argv,FORE);
    if(pid==-1) {
        printFormat("Error creating Philosopher %s\n",philosophersName[i]);
    }
    else {
        philosophers[i]->pid=pid;
    }
}

void getInput() {
    semaphore_wait(MUTEX);
    if(getKeyState('r')) {
        if(philoAmount > MIN_PHILOSOPHERS) {
        int pid = philosophers[philoAmount - 1]->pid;
        philoAmount--;
        killProcess(pid);
        }
        else {
            printFormat("Minimum philosophers reached\n");
        }
    } else if(getKeyState('a')) {
        if(philoAmount < MAX_PHILOSOPHERS) {
            addPhilo(philoAmount);
        }
        else {
            printFormat("Max philosophers reached\n");
        }
    }
    semaphore_post(MUTEX);
}

void eat(int philoNumber) {
    wait(55 * (1+ philoNumber));
}

void think(int philoNumber) {
    wait(110 * (1+ philoNumber));
}

void takeForks(int philoNum) {
    semaphore_wait(MUTEX);
    philosophers[philoNum]->state = HUNGRY;
    test(philoNum);
    semaphore_post(MUTEX);
    semaphore_wait(philosophers[philoNum]->sem);
}

void putForks(int philoNum) {
    semaphore_wait(MUTEX);
    philosophers[philoNum]->state = THINKING;
    test((philoNum + philoAmount - 1) % philoAmount);
    test((philoNum + 1) % philoAmount);
    semaphore_post(MUTEX);
}

void test(int philoNum){
    if((philosophers[philoNum]->state == HUNGRY) && (philosophers[(philoNum + philoAmount - 1) % philoAmount]->state != EATING) && (philosophers[(philoNum + 1) % philoAmount]->state != EATING)) {
        philosophers[philoNum]->state=EATING;
        semaphore_post(philosophers[philoNum]->sem);
    }
}
