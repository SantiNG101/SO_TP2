
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

int64_t init_phylo_reunion(int argc, char* argv[]);

char philosophersName [MAX_PHILOSOPHERS][24]={"Aristoteles","Platon","Socrates","Descartes","Kant","Nietzsche","Sartre","Russell","Machiavelli"};

void get_input();
void add_philo(int i);
void test(int philoNum);
void take_forks(int philoNum);
void putForks(int philoNum);
void eat(int philoNumber);
void think(int philoNumber);

typedef struct Philosopher {
    char name[24];
    int state;
    int pid;
    int64_t sem;
}Philosopher;

int philoAmount = 0;
typedef struct Philosopher* philo;

static philo philosophers[MAX_PHILOSOPHERS];
int64_t sem = 0;


void philosopher_activity(int argc, char* argv[]) {
    int philoNumber = argv[1]; 
    while(1) {
        think(philoNumber);
        take_forks(philoNumber);
        eat(philoNumber);
        putForks(philoNumber);
    }
}

int64_t init_phylo_reunion(int argc, char* argv[]) {
    sem = create_semaphore(MUTEX, 1);
    philoAmount=0;

    semaphore_wait(sem);
    for(int i=0;i<INIT_PHILOSOPHERS;i++) {
        add_philo(i);
        philoAmount++;
    }
    semaphore_post(sem);
    while(1) {
        get_input();
        wait_time(2);
        semaphore_wait(sem);
        for(int i=0;i<philoAmount;i++) {
            if(philosophers[i]->state == EATING) {
                printf("E");
            }
            else {
                printf(".");
            }
        }
        printf("\n");
        semaphore_post(sem);
    }
}

void add_philo(int i) {
    philosophers[i] = alloc(sizeof(Philosopher));
    if(philosophers[i]==NULL) {
        printf("Error creating Philosopher %s\n",philosophersName[i]);
        return -1;
    }
    strcpy(philosophers[i]->name,philosophersName[i]);
    philosophers[i]->state = THINKING;
    philosophers[i]->sem = create_semaphore(philosophersName[i], 1);
    char* argv[] = {philosophersName[i], philoAmount};
    int pid= execve( getpid(),philosopher_activity,1,argv,FORE);
    if(pid==-1) {
        printf("Error creating Philosopher %s\n",philosophersName[i]);
    }
    else {
        philosophers[i]->pid=pid;
    }
}

void get_input() {
    semaphore_wait(sem);
    if ( get_buffer_status() ){
        char c = getChar();
        if(c == 'r') {
            if(philoAmount > MIN_PHILOSOPHERS) {
            int pid = philosophers[philoAmount - 1]->pid;
            philoAmount--;
            kill_process(pid);
            }
            else {
                printf("Minimum philosophers reached\n");
            }
        } else if(c == 'a') {
            if(philoAmount < MAX_PHILOSOPHERS) {
                add_philo(philoAmount);
                philoAmount++;
            }
            else {
                printf("Max philosophers reached\n");
            }
        }
    }
    semaphore_post(sem);
}

void eat(int philoNumber) {
    wait_time(1 + philoNumber);
}

void think(int philoNumber) {
    wait_time(1 + philoNumber);
}

void take_forks(int philoNum) {
    semaphore_wait(sem);
    philosophers[philoNum]->state = HUNGRY;
    test(philoNum);
    semaphore_post(sem);
    semaphore_wait(philosophers[philoNum]->sem);
}

void putForks(int philoNum) {
    semaphore_wait(sem);
    philosophers[philoNum]->state = THINKING;
    test((philoNum + philoAmount - 1) % philoAmount);
    test((philoNum + 1) % philoAmount);
    semaphore_post(sem);
}

void test(int philoNum){
    if((philosophers[philoNum]->state == HUNGRY) && (philosophers[(philoNum + philoAmount - 1) % philoAmount]->state != EATING) && (philosophers[(philoNum + 1) % philoAmount]->state != EATING)) {
        philosophers[philoNum]->state=EATING;
        semaphore_post(philosophers[philoNum]->sem);
    }
}
