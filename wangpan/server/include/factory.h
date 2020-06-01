#ifndef __FACTORY_H__
#define __FACTORY_H__
#include <mysql/mysql.h>
#include "work_queue.h"
typedef struct{
    int dataLen;
    char buf[1000];
}train_t;

typedef struct{
    Queue_t queue;
    pthread_t *pthid;
    pthread_cond_t cond;
    int threadNumber;
    short starFlag;
    MYSQL *conn;
}Factory_t,*pFactory_t;
int factoryInit(pFactory_t,int,int);
int factoryStart(pFactory_t);

int transferFile(int newFd);
int tcpInit(int* sfd,char* ip,char* port);
int epollInAdd(int epid,int fd);
int recvCycle(int fd,void *buf,off_t recvLen);

#endif
