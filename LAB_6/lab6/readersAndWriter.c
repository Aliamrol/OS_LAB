#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>



int segment_id;


sem_t *write_lock;

pthread_mutex_t mutex;


#define SHM_KEY (key_t)1097
#define SHM_PERM  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#define SHM_SIZE sizeof(int)

int shmFlags = IPC_CREAT | SHM_PERM;

void writerr() {
    int *count;
    int write_count = 0;

    if ((segment_id = shmget(SHM_KEY, SHM_SIZE, shmFlags)) < 0 ) {
        perror("SHMSTAT: shmget");
        exit(EXIT_FAILURE);
    }
    count = (int *)shmat(segment_id, NULL, 0);


    while (write_count < 20){
        write_count++;
        *count = write_count;
        printf("writer count: %d, pid: %d\n",*count, getpid());
    }

    sem_post(write_lock);
}

int read_count = 0;

void reader(int id) {

    sem_wait(write_lock);


    int *count = NULL;
    if ((segment_id = shmget(SHM_KEY, SHM_SIZE, 0444)) < 0 ) {
        perror("SHMSTAT: shmget");
        exit(EXIT_FAILURE);
    }

    count = (int *) shmat(segment_id, NULL, 0);
    read_count++;
    printf("Reader %d, count: %d, pid: %d\n", id, *count, getpid());
    printf("read_count: %d\n", read_count);

    sem_post(write_lock);
}


int main() {
    int *count = 0;
    if ((segment_id = shmget(SHM_KEY, SHM_SIZE, shmFlags)) < 0 ) {
        perror("SHMSTAT: shmget");
        exit(EXIT_FAILURE);
    }
    count = (int *)shmat(segment_id, NULL, 0);

    pthread_t writer;

    pthread_t reader1, reader2;

    pthread_mutex_init(&mutex, NULL);
    write_lock = sem_open("/aaaaaa", O_CREAT, 0644, 0);

    pthread_create(&reader1, NULL, (void *)reader, (int *)1);
    pthread_create(&reader2, NULL, (void *)reader, (int *)2);

    pthread_create(&writer, NULL, (void *)writerr, (void *)1);


    pthread_join(writer, NULL);

    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);

    sem_close(write_lock);
    pthread_mutex_destroy(&mutex);

    shmdt(count);
    return 0;
}