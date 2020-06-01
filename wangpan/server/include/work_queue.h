#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include "head.h"

typedef struct tag_node{
    int newFd;
    struct tag_node *pNodeNext;
    int precode;
    /* char absolutePath[1024]; */
    char userName[128];
    struct sockaddr_in client;
}Node_t,*pNode_t;
typedef struct tag_queue{
    pNode_t pHead,pTail;
    int queueCapacity;
    int queueSize;
    pthread_mutex_t queueMutex;
}Queue_t,*pQueue_t;
void queueInit(pQueue_t,int);
void queueSet(pQueue_t,pNode_t);
int queueGet(pQueue_t,pNode_t*);
#endif
