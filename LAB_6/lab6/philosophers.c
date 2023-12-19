#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PHILOSOPERS_NUM 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT_PHILOSOPHER (philosopher_number+4)%PHILOSOPERS_NUM
#define RIGHT_PHILOSOPHER (philosopher_number+1)%PHILOSOPERS_NUM

pthread_mutex_t mutex;
pthread_cond_t cond_var[PHILOSOPERS_NUM];
pthread_t threadID[PHILOSOPERS_NUM];


void * philospher(void *num);
void pickup_forks(int);
void return_forks(int);
void philosopher_can_eat(int);
int state[PHILOSOPERS_NUM];
int philosophers[PHILOSOPERS_NUM]={0,1,2,3,4};

void *philospher(void *num) {
    while(1) {
        int *i = num;
        int x = rand() % 3;
        printf("philosopher %d is thinking !!\n", *i + 1);
        sleep(x+1);
        pickup_forks(*i);
        return_forks(*i);
    }
}

void pickup_forks(int philosopher_number) {

    pthread_mutex_lock (&mutex);

    state[philosopher_number] = HUNGRY;
    philosopher_can_eat(philosopher_number);
    if (state[philosopher_number] != EATING){
        pthread_cond_wait(&cond_var[philosopher_number], &mutex);
    }

    pthread_mutex_unlock (&mutex);
}

void return_forks(int philosopher_number) {

    pthread_mutex_lock (&mutex);

    state[philosopher_number] = THINKING;
    printf("philosopher %d finished eating !!\n", philosopher_number+1);
    philosopher_can_eat(LEFT_PHILOSOPHER);
    philosopher_can_eat(RIGHT_PHILOSOPHER);

    pthread_mutex_unlock (&mutex);
}

void philosopher_can_eat(int philosopher_number) {

    if (state[philosopher_number] == HUNGRY && state[LEFT_PHILOSOPHER] != EATING && state[RIGHT_PHILOSOPHER] != EATING) {
        state[philosopher_number] = EATING;
        printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n", philosopher_number+1, LEFT_PHILOSOPHER+1, philosopher_number+1);
        pthread_cond_signal(&cond_var[philosopher_number]);
    }
}

int main() {
    int i;
    time_t t;
    srand((unsigned) time(&t));
    pthread_t threadID[PHILOSOPERS_NUM];

    for(i=0; i < PHILOSOPERS_NUM; i++) {
        state[i] = THINKING;
        pthread_cond_init(&cond_var[i], NULL);
    }
    pthread_mutex_init (&mutex, NULL);
    for(i=0; i < PHILOSOPERS_NUM; i++) {

        pthread_create(&threadID[i], NULL, philospher, &philosophers[i]);
    }
    for(i=0; i<PHILOSOPERS_NUM; i++) {
        pthread_join(threadID[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
}