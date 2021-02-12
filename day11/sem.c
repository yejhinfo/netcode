#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)
/*
 int semget(key_t key, int nsems, int semflg);
int semctl(int semid, int semnum, int cmd, ...);
int semop(int semid, struct sembuf *sops, size_t nsops);



*/

union semun {
               int              val;    // Value for SETVAL 
               struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET 
               unsigned short  *array;  // Array for GETALL, SETALL 
               struct seminfo  *__buf;  // Buffer for IPC_INFO 
           };

int sem_creat(key_t key){
    int semid =  semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if(semid == -1){
        ERR_EXIT("semget");
    }
    return semid;
}

int sem_open(key_t key){
    int semid =  semget(key, 1, 0666);
    if(semid == -1){
        ERR_EXIT("semget");
    }
    return semid;
}


int sem_setval(int semid, int val){
    int ret = 0;
    union semun su;
    su.val = val;
    ret = semctl(semid, 0, SETVAL, su);
    if(ret == -1){
        ERR_EXIT("semctl");
    }

    return ret;
}

int sem_getval(int semid){
    int ret = 0;
    union semun su;

    ret = semctl(semid, 0, GETVAL, su);
    if(ret == -1){
        ERR_EXIT("semctl");
    }
    printf("su.val: %d\n",su.val);

    return ret;
}


int sem_p(int semid){
    struct sembuf sops = {0,-1,0};
    int ret = semop(semid, &sops, 1);

    return ret;
}


int sem_v(int semid){
    struct sembuf sops = {0,1,0};
    int ret = semop(semid, &sops, 1);
    return ret;
}


int main(int argc, char const *argv[])
{
    int semid;
    semid = sem_open(0x1111);
    sem_setval(semid,1);
    sem_getval(semid); 

    fork();
    int i = 0;

    sem_p(semid);
        printf("i: %d ,pid : %d\n",i++,getpid());
        sleep(3);
        printf("i: %d ,pid : %d\n",i++,getpid());
    sem_v(semid);
    return 0;
}
