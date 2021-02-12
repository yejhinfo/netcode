#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>


/*
   //表示打开消息队列，不存在报错
     int msgget(key_t key, int msgflg);
*/

int main(int argc, char const *argv[])
{
    int getpid;
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
    printf("hello\n");
    return 0;
}
