#include "header.h"

int main()
{
    union Semun sharedMemSemData;
    union Semun emptySemData;
    union Semun fullSemData;
    int rem=0;
    int shmId;
    int *shmAddr;
    shmAddr = initSharedMem('a', IPC_CREAT | IPC_EXCL| 0666, &shmId);
    int sharedMemSem = semget('b', 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (sharedMemSem == -1)
    {
        sharedMemSem = semget('b', 1, 0666 | IPC_CREAT);
    }
    else
    {
        sharedMemSemData.val = 1;
        if (semctl(sharedMemSem, 0, SETVAL, sharedMemSemData) == -1)
        {
            perror("Error in semctl");
            exit(-1);
        }
        else{
            printf("process with pid %d has initialzed sharedsem\n",getpid());
        }
    }


    int emptySem = semget('e', 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (emptySem == -1)
    {
        emptySem = semget('e', 1, 0666 | IPC_CREAT);
    }
    else
    {
        emptySemData.val = 3;
        if (semctl(emptySem, 0, SETVAL, emptySemData) == -1)
        {
            perror("Error in semctl");
            exit(-1);
        }
        else{
            printf("process with pid %d has initialzed emptySem\n",getpid());
        }
    }
     int fullSem = semget('f', 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (fullSem == -1)
    {
        fullSem = semget('f', 1, 0666 | IPC_CREAT);
    }
    else
    {
        fullSemData.val = 0;
        if (semctl(fullSem, 0, SETVAL, fullSemData) == -1)
        {
            perror("Error in semctl");
            exit(-1);
        }
        else{
            printf("process with pid %d has initialzed fullSem\n",getpid());
        }
    }
    while (1)
    {
    down(fullSem);
    down(sharedMemSem);
    shmAddr[rem]=-1;
    printf ("Consume value at position %d\n", rem);
    rem = (rem+1) % N;
    up(sharedMemSem);
    up(emptySem);
    sleep(2);   //simulate that consumer slower than producer
    }
    return 0;
}