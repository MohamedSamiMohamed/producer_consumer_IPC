
#include "header.h"

int *shmAddr;
int shmId;
int sharedMemSem;
int emptySem;
int fullSem;
int *addAddr;
int shmAddId;
int *remAddr;
int shmRemId;

struct shmid_ds shmData;
void handler(int segNum){
shmdt(shmAddr);
shmdt(remAddr);
shmdt(addAddr);
shmctl(shmId, IPC_STAT,&shmData);
if(shmData.shm_nattch==0){
printf("Producer releasing IPC\n");
semctl(sharedMemSem,0,IPC_RMID,(union Semun*)0);
semctl(fullSem,0,IPC_RMID,(union Semun*)0);
semctl(emptySem,0,IPC_RMID,(union Semun*)0);
shmctl(shmId, IPC_RMID,(struct shmid_ds*)0);
shmctl(shmAddId, IPC_RMID,(struct shmid_ds*)0);
shmctl(shmRemId, IPC_RMID,(struct shmid_ds*)0);

}
exit(0);
signal (SIGINT, handler);
}
int main()
{
    signal (SIGINT, handler);
    union Semun sharedMemSemData;
    union Semun emptySemData;
    union Semun fullSemData;
    shmAddr = initSharedMem('a', IPC_CREAT | 0666, &shmId,N);
    sharedMemSem = semget('b', 1, 0666 | IPC_CREAT | IPC_EXCL);
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
        else
        {
            printf("process with pid %d has initialzed sharedsem\n", getpid());
        }
    }

    addAddr = initSharedMem('c', IPC_CREAT|IPC_EXCL | 0666, &shmAddId,1);
    remAddr = initSharedMem('r', IPC_CREAT|IPC_EXCL | 0666, &shmRemId,1);

    emptySem = semget('e', 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (emptySem == -1)
    {
        emptySem = semget('e', 1, 0666 | IPC_CREAT);
    }
    else
    {
        emptySemData.val = N;
        if (semctl(emptySem, 0, SETVAL, emptySemData) == -1)
        {
            perror("Error in semctl");
            exit(-1);
        }
        else
        {
            printf("process with pid %d has initialzed sharedsem\n", getpid());
        }
    }
    fullSem = semget('f', 1, 0666 | IPC_CREAT | IPC_EXCL);
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
        else
        {
            printf("process with pid %d has initialzed sharedsem\n", getpid());
        }
    }
    while (1)
    {   
        down(emptySem);
        down(sharedMemSem);
        shmAddr[*addAddr]=*addAddr;
        printf ("producer: inserted at position %d\n",*addAddr);
        *addAddr = (*addAddr+1) % N;
        up(sharedMemSem);
        up(fullSem);
        sleep(1);
    }
    return 0;
}