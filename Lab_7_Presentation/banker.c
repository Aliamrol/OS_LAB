#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_CUSTOMERS 10
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need [NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t bankers;
int request_resources(int, int[]);
int release_resources(int, int[]);
void* customer(void*);
bool is_safe();




int main(int argc, char** argv){

    int res = pthread_mutex_init(&bankers, NULL);
    if (res != 0){
        printf("failed to initialize the mutex\n");
        return 1;
    }

    srand(0);

    if (argc < NUMBER_OF_RESOURCES + 1){
        printf("not enough arguements\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] = strtol(argv[i+1], NULL, 10);
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("av[%d]:%d\n", i, available[i]);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            allocation[i][j] = 0;
            maximum[i][j] = 1 + rand() % (available[j]);
            need[i][j] = maximum[i][j];
        }
    }

    int id[NUMBER_OF_CUSTOMERS];
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        id[i] = i;
    }
    pthread_t tids[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        pthread_create(&tids[i], NULL, customer, (void*)(pthread_t)&id[i]);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(tids[i], NULL);
    }


    return 0;
    

}


int release_resources(int customer_id, int request[]){
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > allocation[customer_id][i]){
            return -1;
        }
    }
    // lock this section, no other process can enter this while another one is inside
    pthread_mutex_lock(&bankers);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] += request[i];
        allocation[customer_id][i] -= request[i];
    }
    printf("customer %d released the followings: ", customer_id);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("%d ", request[i]);
    }
    printf("  :: available resources -> ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("%d ", available[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&bankers);
    return 0;
}


int request_resources(int customer_id, int request[]){
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > need[customer_id][i]){
            return -1;
        }
    }

    pthread_mutex_lock(&bankers);
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        available[i] -= request[i];
        allocation[customer_id][i] += request[i];
        need[customer_id][i] -= request[i];
    }
    if (!is_safe()){
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] += request[i];
            allocation[customer_id][i] -= request[i];
            need[customer_id][i] += request[i];
        }
        pthread_mutex_unlock(&bankers);
        return -1;
    }

    printf("customer %d took the followings ", customer_id);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("%d ", request[i]);
    }
    printf("  :: available -> ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        printf("%d ", available[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&bankers);
    return 0;

}


bool is_safe(){
    // copy allocation to work
    int work[NUMBER_OF_RESOURCES];
    bool finished[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        finished[i] = false;
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        work[i] = available[i];
    };

    for (int cid = 0; cid < NUMBER_OF_CUSTOMERS; cid++){
        if (finished[cid]){
            continue;
        }
        bool enough = true;
        for (int rid = 0; rid < NUMBER_OF_RESOURCES; rid++){
            if (need[cid][rid] > work[rid]){
                enough = false;
                break;
            }
        }
        if (!enough){
            continue;
        }
        finished[cid] = true;
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            work[i] += allocation[cid][i];
        }
        cid = -1;
    } 

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        if (!finished[i]){
            return false;
        }
    }
    return true;
}


bool is_done(int customer_id){
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (need[customer_id][i] > 0){
            return false;
        }
    }
    return true;
}


void* customer(void* pid){
    int customer_id = *(int*)pid;

    while(true){

        sleep(2);
        int request[NUMBER_OF_RESOURCES];
        bool not_zero = true;
        if (is_done(customer_id)){
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
                request[i] = allocation[customer_id][i];
            }
            
            release_resources(customer_id, request);
            
            printf("[INFO]:customer : %d is done with transactions\n", customer_id);
            pthread_exit(NULL);
        }
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            request[i] = rand() % (allocation[customer_id][i] + 1);
            if (request[i] != 0){
                not_zero = false;
            }
        }
        if (!not_zero){
            int res = release_resources(customer_id, request);
        }
        not_zero = true;
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            request[i] = rand() % (need[customer_id][i] + 1);
            if (request[i] != 0){
                not_zero = false;
            }
        }
        if (!not_zero){
            int res = request_resources(customer_id, request);
        }
        
    }


    
    
}

