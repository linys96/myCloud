#include "../include/clientFtpCmd.h"
#include "../include/color.h"
int clientFtpCmd(int sockfd,train_t *pTrain)
{
    int ret;
    char cmdName[128]={0};
    char tmp[1024]={0};
    sscanf(pTrain->buf,"%s%s",cmdName,tmp);
    if(0==strcmp(cmdName,"cd"))
    {
        recvCycle(sockfd,&ret,4);
        if(ret)
        {
            printf("cd error: %s: Not a directory\n",tmp);
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"ls"))
    { 
        recvCycle(sockfd,&pTrain->dataLen,sizeof(int));
        //ls结果可能有多条，所以要循环接
        int flag=1;
        while(pTrain->dataLen>0)
        {
            if(flag)
            {
                printf(L_BLUE"  %4s %12s %12s\n" NONE,"type","filename","size(byte)");
                flag=0;
            }
            bzero(pTrain->buf,sizeof(pTrain->buf));
            recvCycle(sockfd,pTrain->buf,pTrain->dataLen);
            printf("%s\n",pTrain->buf);
            recvCycle(sockfd,&pTrain->dataLen,sizeof(int));
            /* write(STDOUT_FILENO,pTrain->buf,pTrain->dataLen); */
        }
    }
    else if(0==strcmp(cmdName,"puts"))
    {
        ret=cmdPuts(sockfd,tmp);
        if(-1==ret)
        {
            return -1;
        }

    }
    else if(0==strcmp(cmdName,"gets"))
    {
        ret=cmdGets(sockfd,tmp);
        if(-1==ret)
        {
            printf("gets error\n");
            return -1;
        }

    }
    else if(0==strcmp(cmdName,"remove"))
    {
        recvCycle(sockfd,&ret,4);
        if(-1==ret)
        {
            printf("remove error: No such file or directory\n");
            return -1;
        }
        /* else if(-2==ret) */
        /* { */
        /*     printf("remove error: %s is a directory\n",tmp); */
        /*     return -1; */
        /* } */
    }
    else if(0==strcmp(cmdName,"pwd"))
    {
        bzero(pTrain,sizeof(train_t));
        recvCycle(sockfd,&pTrain->dataLen,sizeof(int));
        recvCycle(sockfd,pTrain->buf,pTrain->dataLen);
        printf("%s\n",pTrain->buf);
    }
    else if(0==strcmp(cmdName,"mkdir"))
    {
        recvCycle(sockfd,&ret,4);
        if(ret)
        {
            bzero(pTrain,sizeof(train_t));
            recvCycle(sockfd,&pTrain->dataLen,sizeof(int));
            recvCycle(sockfd,pTrain->buf,pTrain->dataLen);
            printf("mkdir error: cannot create directory ‘%s’: File exists\n",pTrain->buf);
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"help"))
    {
        printf(L_BLUE"===========>>\n" NONE);
        printf(BLUE"cd:" NONE" change the working directory\n");
        printf(BLUE"ls:" NONE" list information about the files in the current directory\n");
        printf(BLUE"puts:" NONE" upload the file to the server\n");
        printf(BLUE"gets:" NONE" download the file from the server\n");
        printf(BLUE"remove:" NONE" delete file or directory from the server\n");
        printf(BLUE"pwd:" NONE"print the current working directory\n");
        printf(BLUE"mkdir:" NONE"create the directory\n");
        printf(BLUE"help:" NONE"print help information\n");
        printf(BLUE"exit:" NONE"cause the client to exit\n");
        printf(L_BLUE"===========>>\n" NONE);
    }
    else if(0==strcmp(cmdName,"exit"))
    {
        exit(0);
    }
    /* printf("Comand finish\n"); */

    return 0;
}
