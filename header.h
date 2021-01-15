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

int* initSharedMem(int key,int perm,int *shmId,int size){
    key_t shmKey=ftok("key",key);
    if(perm==IPC_CREAT|IPC_EXCL|0666){
    *shmId=shmget(shmKey,size*sizeof(int),perm);
    if(*shmId==-1){
    *shmId=shmget(shmKey,size*sizeof(int),IPC_CREAT|0666);
    return attachShm(*shmId);
    }
    else{
    int *addr=attachShm(*shmId);
    *addr=0;
    return addr;  
    }  
    }
    else{
    
    *shmId=shmget(shmKey,size*sizeof(int),perm);
    return attachShm(*shmId);
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