#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>



int main() {


    int shmid = shmget(40, 100, 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }


    char *shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while(true){
        char phrase[50];
        fgets(phrase, 50, stdin);
        sprintf(shmaddr, phrase);
    }

    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}