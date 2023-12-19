#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>



int main() {

    int shmid = shmget(40, 100, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }


    char *shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    char tmp[50];
    while(true){
        if (strcmp(shmaddr, tmp) != 0){
            printf("%s", shmaddr);
            strcpy(tmp, shmaddr);
        }
    }

    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}





