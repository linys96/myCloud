#include "../include/factory.h"
#include "../include/ftpCmd.h"
extern int connectedNumber;
//线程清理函数
void cleanupFunc(void *p)
{
    pQueue_t pQueue=(pQueue_t)p;
    printf("Unlock success\n");
    pthread_mutex_unlock(&pQueue->queueMutex);
}
//线程函数
void* threadFunc(void *p)
{
    pFactory_t pFactoryInfo=(pFactory_t)p;
    pQueue_t pQueue=&pFactoryInfo->queue;
    pthread_cleanup_push(cleanupFunc,pQueue);
    pNode_t pGet;
    int ret;
    int getFlag,exitFlag,choice;
    train_t *pTrain=(train_t *)calloc(1,sizeof(train_t));
    while(1)
    {
        if(0==pFactoryInfo->starFlag)
        {
            printf("Thread exit success\n");
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&pQueue->queueMutex);
        if(!pQueue->queueSize)
        {
            pthread_cond_wait(&pFactoryInfo->cond,&pQueue->queueMutex);
        }
        getFlag=queueGet(pQueue,&pGet);
        pthread_mutex_unlock(&pQueue->queueMutex);
        if(!getFlag)
        {
            //验证账户密码
            while(1)
            {
                exitFlag=0;
                recvCycle(pGet->newFd,&choice,4);
                switch(choice)
                {
                case 0: return 0;
                case 1:
                        do{ 
                            ret=logIn(pGet->userName,pGet->newFd,pFactoryInfo->conn);
                        }while(ret);
                        exitFlag=1;
                        break;
                case 2:
                        do{ 
                            ret=signUp(pGet->userName,pGet->newFd,pFactoryInfo->conn);
                        }while(ret);
                        exitFlag=1;
                        break;
                }
                if(exitFlag)
                {
                    break;
                }
            }
            connectedNumber++;
            printf("%s connect, %d connecting now\n",pGet->userName,connectedNumber);
            makeLog(pGet->userName,inet_ntoa(pGet->client.sin_addr),"log in",pFactoryInfo->conn);
            cmdPrintPath(pGet->precode,pGet->userName,pGet->newFd,pFactoryInfo->conn);
            //记录登录时间
            /* makeConnectLog(pGet->client); */
            //子线程接到任务处理客户端的命令
            while(1)
            {
                bzero(pTrain,sizeof(train_t));
                ret=recvCycle(pGet->newFd,&pTrain->dataLen,4);
                //ret等于-1说明客户端断开，break
                if(-1==ret)
                {
                    makeLog(pGet->userName,inet_ntoa(pGet->client.sin_addr),"exit",pFactoryInfo->conn);
                    break;
                }
                recvCycle(pGet->newFd,pTrain->buf,pTrain->dataLen);
                makeLog(pGet->userName,inet_ntoa(pGet->client.sin_addr),pTrain->buf,pFactoryInfo->conn);
                ftpCmd(pGet,pTrain,pFactoryInfo->conn); 
                cmdPrintPath(pGet->precode,pGet->userName,pGet->newFd,pFactoryInfo->conn);
            }
            free(pGet);
            pGet=NULL;
            connectedNumber--;
        }
    }
    pthread_cleanup_pop(1);
}
//线程池初始化
int factoryInit(pFactory_t p,int threadNumber,int capacity)
{
    queueInit(&p->queue,capacity);
    pthread_cond_init(&p->cond,NULL);
    p->threadNumber=threadNumber;
    p->pthid=(pthread_t*)calloc(threadNumber,sizeof(pthread_t));
    p->starFlag=0;
    return 0;
}
//创建线程，启动线程池
int factoryStart(pFactory_t p)
{
    if(!p->starFlag)
    {
        int i,ret;
        p->starFlag=1;
        for(i=0;i<p->threadNumber;i++)
        {
            ret=pthread_create(p->pthid+i,NULL,threadFunc,p);
            THREAD_ERROR_CHECK(ret,"pthread_create");
        }
    }
    return 0;
}
