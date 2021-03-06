#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

typedef struct _Teacher
{
	char name[64];
	int age;
} Teacher;


//
/*
int shmget(key_t key, size_t size, int shmflg);
//

PSIS
       #include <sys/ipc.h> #include <sys/shm.h>

       int shmctl(int shmid, int cmd, struct shmid_ds *buf);

DESCRIPTION
       shmctl()  performs  the  control  operation specified by cmd on the shared memory segment whose identifier is
       given in shmid.

       The buf argument is a pointer to a shmid_ds structure, defined in <sys/shm.h> as follows:

           struct shmid_ds {
               struct ipc_perm shm_perm;    // Ownership and permissions //
               size_t          shm_segsz;   // Size of segment (bytes) //
               time_t          shm_atime;   // Last attach time //
               time_t          shm_dtime;   // Last detach time //
               time_t          shm_ctime;   // Last change time //
               pid_t           shm_cpid;    // PID of creator //
               pid_t           shm_lpid;    // PID of last shmat()/shmdt() //
               shmatt_t        shm_nattch;  // No. of current attaches //
               ...
           };

       The ipc_perm structure is defined in <sys/ipc.h> as  follows  (the  highlighted  fields  are  settable  using
       IPC_SET):

           struct ipc_perm {
               key_t key;            // Key supplied to shmget() //
               uid_t uid;            // Effective UID of owner //
               gid_t gid;            // Effective GID of owner //
               uid_t cuid;           // Effective UID of creator //
               gid_t cgid;           // Effective GID of creator //
               unsigned short mode;  // Permissions + SHM_DEST and
                                        SHM_LOCKED flags //
               unsigned short seq;   // Sequence number //
           };
*/

//
//key        shmid      owner      perms      bytes      nattch     status      
//0x00002234 131073     wbm01     666        68         0    
//  
int main(int argc, char *argv[])
{
    int key = 0;
    key = shmget(0x2333, sizeof(Teacher),0666|IPC_CREAT);
    if(key < 0){
        ERR_EXIT("shmget");
    }
    Teacher *p = NULL;
    Teacher t1 = {"tom",34};
    p = shmat(key, NULL, 0);
    memcpy(p,&t1,sizeof(t1)); 

	return 0;	
} 