#ifndef __HEAD_H__
#define __HEAD_H__
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<fcntl.h>
#include<grp.h>
#include<sys/select.h>
#include<sys/time.h>
#include<syslog.h>
#include<sys/mman.h>
#include<strings.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<errno.h>
#include<signal.h>
#include<pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <shadow.h>
#define ARGS_CHECK(argc,val){if(argc!=val){printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName){if(ret==retVal){perror(funcName);return -1;}}
#define VOID_ERROR_CHECK(ret,retVal,funcName){if(ret==retVal){perror(funcName);exit(-1);}}
#define THREAD_ERROR_CHECK(ret,funcName){if(ret!=0){printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define CHILD_THREAD_ERROR_CHECK(ret,funcName){if(ret!=0){printf("%s:%s\n",funcName,strerror(ret));return (void*)-1;}}

typedef struct{
    int dataLen;
    char buf[1000];
}train_t;

int logIn(int socketFd);
int signUp(int socketFd);
int generateRandString(char *str,int len);
int recvCycle(int fd,void *buf,off_t recvLen);
int epollInAdd(int epid,int fd);

#endif
