#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


typedef struct
{
    int data[25];
} Data;

int samples = 500000;
int processes_num = 5;

void printHistogram(int hist[]);

int id;
int parent_pid;
int pid;

int main(int argc, char const *argv[])
{

    Data* data;

    

    if ((id = shmget(IPC_PRIVATE, sizeof(Data), IPC_CREAT | 0666)) < 0)
    {
        printf("something went wrong while creating the memory");
        exit(-1);
    }

    clock_t start;
	start = clock();

    parent_pid = getpid();
    for (int i = 0; i < processes_num; i++)
    {
        //only parent is able to fork the processes
        if (getpid() == parent_pid)
            pid = fork();
        else
            break;
    }
    if ((data = (Data *)shmat(id, NULL, 0)) == (Data *)-1)
    {
        printf("unable to assign the shared memory to process");
        exit(-1);
    }

    if (getpid() == parent_pid)
    {
        for (int i = 0; i < processes_num; i++)
        {
            wait(NULL);
        }
    }
    else
    {

        int counter, random;
        for (int i = 0; i < samples / processes_num; i++)
        {
            counter = 0;
            for (int j = 0; j < 12; j++)
            {
                random = rand() % 101;
                if (random >= 49)
                {
                    counter++;
                }
                else
                {
                    counter--;
                }
            }
            data->data[counter + 12]++;
        }

        

        exit(0);
    }


    clock_t end;
    end = clock();

    printf("%f", (float)(end - start)/CLOCKS_PER_SEC);

    // printHistogram(hist->data);

    return 0;
}

void printHistogram(int hist[])
{
    for (int i = 0; i < 25; i++)
    {
        for(int j = 0; j < hist[i]; j++) {
          printf("*");
        }
        printf("\n");
    }
}