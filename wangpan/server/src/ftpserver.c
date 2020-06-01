#include "../include/factory.h"
#include "../include/ftpCmd.h"

int connectedNumber=0;//连接的客户端数
int exitFds[2];
void sigExitFunc(int signum)
{
    printf("Ready to exit\n");
    write(exitFds[1],&signum,1);
}
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("./ftpserver ../conf/config");
        return -1;
    }
    
    pipe(exitFds);
    int status;
    //创建子进程，子进程启动线程池
    while(fork())
    {
        signal(SIGUSR1,sigExitFunc);
        wait(&status);
        //如果子进程是正常退出，父进程也退出
        //否则父进程通过fork重新拉起子进程
        if(WIFEXITED(status))
        {
            printf("ThreadPool exit success\n");
            exit(0);
        }
    }
    //读配置文件，将内容存到对应的变量
    int ret;
    configuration_t config;
    bzero(&config,sizeof(configuration_t));
    ret=readConfigurationFile(argv[1],&config);
    if(-1==ret)
    {
        return -1;
    }
    Factory_t factoryInfo;
    //连接数据库
    factoryInfo.conn=mysql_init(NULL);
    if(!mysql_real_connect(factoryInfo.conn,config.server,
        config.user,config.password,config.database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(factoryInfo.conn));
        return -1;
    }
    else
    {
        printf("Database is connected\n");
    }
    //启动线程池
    factoryInit(&factoryInfo,atoi(config.threadNumber),atoi(config.capacity));
    factoryStart(&factoryInfo); 
    //在上层目录创建一个file目录存放文件
    if(access("../file",F_OK))
    {
        mkdir("../file",0775);
    }
    //建立tcp连接
    int socketFd,newFd;
    tcpInit(&socketFd,config.ip,config.port);
    pQueue_t pq=&factoryInfo.queue;
    //epoll监听socketfd和退出信号，后面可能还要再监听newfd
    int epid;
    epid=epoll_create(1);
    ERROR_CHECK(epid,-1,"epoll_create");
    struct epoll_event *pEvent;
    pEvent=(struct epoll_event *)calloc(2,sizeof(struct epoll_event));
    ret=epollInAdd(epid,socketFd);
    if(-1==ret)
    {
        return -1;
    }
    ret=epollInAdd(epid,exitFds[0]);
    if(-1==ret)
    {
        return -1;
    }
    int readyFdNumber;
    int i,j;
    /* char currentPath[1024]={0}; */
    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    socklen_t clientLen=sizeof(client);
    /* getcwd(currentPath,sizeof(currentPath));//获取当前路径 */
    while(1)
    {
        readyFdNumber=epoll_wait(epid,pEvent,2,-1);
        for(i=0;i<readyFdNumber;i++)
        {
            //退出机制相关
            if(pEvent[i].data.fd==exitFds[0])
            {
                factoryInfo.starFlag=0;
                pthread_cond_broadcast(&factoryInfo.cond);
                for(j=0;j<atoi(config.threadNumber);j++)
                {
                    pthread_join(factoryInfo.pthid[j],NULL);
                }
                printf("MainThead exit success\n");
                exit(0);
            }
            //若有客户端连接放入队列
            if(pEvent[i].data.fd==socketFd)
            {
                newFd=accept(socketFd,(struct sockaddr*)&client,&clientLen);
                pNode_t pNew;
                pNew=(pNode_t)calloc(1,sizeof(Node_t));
                pNew->newFd=newFd;
                memcpy(&pNew->client,&client,sizeof(client));
                pNew->precode=0;
                pthread_mutex_lock(&pq->queueMutex);
                queueSet(pq,pNew);
                pthread_mutex_unlock(&pq->queueMutex);
                pthread_cond_signal(&factoryInfo.cond);
            }
        }
    }
    
    mysql_close(factoryInfo.conn);
    return 0;
}

