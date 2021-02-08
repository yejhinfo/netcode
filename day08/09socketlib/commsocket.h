




#ifndef _SCK_CLINT_H_
#define _SCK_CLINT_H_



#ifdef __cplusplus
extern 'C'
{
#endif



//错误码定义  

#define Sck_Ok             	0
#define Sck_BaseErr   		3000

#define Sck_ErrParam                	(Sck_BaseErr+1)
#define Sck_ErrTimeOut                	(Sck_BaseErr+2)
#define Sck_ErrPeerClosed               (Sck_BaseErr+3)
#define Sck_ErrMalloc			   		(Sck_BaseErr+4)

//函数声明
//客户端环境初始化
int sckCliet_init(void **handle, char *ip, int port, int contime, int sendtime, int revtime);

//int sckCliet_init(void **handle, char *ip, int port, int contime, int sendtime, int revtime);


//客户端发送报文
int sckClient_send(void *handle, unsigned char *data, int datalen);

//客户端端接受报文
int sckClient_rev(void *handle, unsigned char *out, int *outlen); //1
// 客户端环境释放 
int sckClient_destroy(void *handle);


#ifdef __cpluspluse
}
#endif


#endif