#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

   



/*
   //表示打开消息队列，不存在报错
     int msgget(key_t key, int msgflg);
     int msgctl(int msqid, int cmd, struct msqid_ds *buf);

     struct msqid_ds {
               struct ipc_perm msg_perm;     // Ownership and permissions 
               time_t          msg_stime;    // Time of last msgsnd(2) 
               time_t          msg_rtime;    //Time of last msgrcv(2) 
               time_t          msg_ctime;    // Time of last change 
               unsigned long   __msg_cbytes; // Current number of bytes in
                                                queue (nonstandard) 
               msgqnum_t       msg_qnum;     // Current number of messages
                                                in queue 
               msglen_t        msg_qbytes;   // Maximum number of bytes
                                                allowed in queue 
               pid_t           msg_lspid;    // PID of last msgsnd(2) 
               pid_t           msg_lrpid;    // PID of last msgrcv(2) 
           };

       The ipc_perm structure is defined as follows (the highlighted fields are settable using IPC_SET):

           struct ipc_perm {
               key_t          __key;       // Key supplied to msgget(2) 
               uid_t          uid;         // Effective UID of owner 
               gid_t          gid;         // Effective GID of owner 
               uid_t          cuid;        // Effective UID of creator 
               gid_t          cgid;        // Effective GID of creator 
               unsigned short mode;        // Permissions 
               unsigned short __seq;       // Sequence number  
           };

*/
//ipc对象是内核进行初始化的，（我们可以从内核获取信息并且修改
int main(int argc, char const *argv[])
{
    int getpid;
    int ret;
    //表示打开文件要存在，加了IPC_CREAT参数后不存在就创建
    getpid = msgget(0x1234, 0666 | IPC_CREAT | IPC_EXCL);
    if(getpid < 0){
        if(errno == ENOENT){
            printf("no file \n");
        }
         if(errno == EEXIST){
            printf("file EEXIST \n");
        }
        perror("msgget\n");
        return -1;
    }
    printf("msgpid: %d\n",getpid);
    struct msqid_ds msgbuf;
    ret = msgctl(getpid,IPC_STAT, &msgbuf); 
    printf("mode :%o\n",msgbuf.msg_perm.mode);

    printf("hello\n");
    return ret;
}
