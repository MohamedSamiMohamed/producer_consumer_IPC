#ifndef HEADER
#define HEADER

#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
# define N 100



int * attachShm(int shmId){
    return shmat(shmId, (void *)0, 0);
}

int* initSharedMem(int key,int perm,int *shmId){
    key_t shmKey=ftok("key",key);
    *shmId=shmget(shmKey,N*sizeof(int),perm);
    if(*shmId==-1){
        *shmId=shmget(shmKey,N*sizeof(int),0666|IPC_CREAT);
        return attachShm(*shmId);
    }
    else{
        int *shmAddr;
        shmAddr=attachShm(*shmId);
        for (int i=0;i<N;i++){
            shmAddr[i]=-1;
        }
        return shmAddr;
    }
    
}

union Semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;  
    struct seminfo *__buf;
    void *__pad;
};

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}





#endif