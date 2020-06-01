#include "../include/work_queue.h"
//队列初始化
void queueInit(pQueue_t pq,int capacity)
{
    bzero(pq,sizeof(Queue_t));
    pq->queueCapacity=capacity;
    pthread_mutex_init(&pq->queueMutex,NULL);
    
}
//尾插法插入队列
void queueSet(pQueue_t pq,pNode_t pNew)
{
    if(NULL==pq->pHead)
    {
        pq->pHead=pq->pTail=pNew;
    }else{
        pq->pTail->pNodeNext=pNew;
        pq->pTail=pNew;
    }
    pq->queueSize++;
}
//取出队列头结点
int queueGet(pQueue_t pq,pNode_t *ppDelete)
{
    if(NULL==pq->pHead)
    {
        return -1;
    }
    *ppDelete=pq->pHead;
    pq->pHead=pq->pHead->pNodeNext;
    if(NULL==pq->pHead)
    {
        pq->pTail=NULL;
    }
    pq->queueSize--;
    return 0;
}

