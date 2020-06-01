#include "../include/ftpCmd.h"

int ftpCmd(pNode_t pNode,train_t *pTrain,MYSQL *conn)
{
    int ret;
    char cmdName[128]={0};
    char tmp[256]={0};
    /* cmdPrintPath(pNode->precode,pNode->userName,pNode->newFd,conn); */
    sscanf(pTrain->buf,"%s%s",cmdName,tmp);//接命令和参数
    /* makeCmdLog(pNode->client,pTrain->buf); */
    if(0==strcmp(cmdName,"cd"))
    {
        //每个if开头的send是将接收到的命令再发给客户端,下同
        //客户端也有个类似的ftpCmd函数处理不同的命令
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        /* printf("cmd=%s,tmp=%s\n",cmdName,tmp); */
        cmdCd(&pNode->precode,tmp,pNode->newFd,conn);
    }
    else if(0==strcmp(cmdName,"ls"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        ret=cmdLs(pNode->newFd,pNode->precode,pNode->userName,conn);
        if(-1==ret)
        {
            send(pNode->newFd,&ret,4,0);
            /* printf("ls error\n"); */
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"puts"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        ret=cmdPuts(pNode->newFd,pNode->precode,pNode->userName,conn);
        if(-1==ret)
        {
            /* printf("puts error\n"); */
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"gets"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        ret=cmdGets(pNode->newFd,pNode->precode,tmp,pNode->userName,conn);
        if(-1==ret)
        {
            /* printf("gets error\n"); */
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"remove"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        ret=cmdRemove(pNode->precode,tmp,pNode->userName,pNode->newFd,conn);
        if(0==ret)
        {
            send(pNode->newFd,&ret,4,0);
        }
    }
    else if(0==strcmp(cmdName,"pwd"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        ret=cmdPwd(pNode->precode,pNode->newFd,conn);
        if(-1==ret)
        {
            /* printf("pwd error\n"); */
            return -1;
        }
    }
    else if(0==strcmp(cmdName,"mkdir"))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
        //mkdir的参数可能不止一个，所以要处理一下
        strcpy(tmp,pTrain->buf+strlen(cmdName)+1);
        ret=cmdMkdir(pNode->precode,tmp,pNode->userName,pNode->newFd,conn);
        if(-1==ret)
        {
            /* printf("mdkir error\n"); */
            return -1;
        }
    }
    else if((0==strcmp(cmdName,"help"))||(0==strcmp(cmdName,"exit")))
    {
        send(pNode->newFd,pTrain,pTrain->dataLen+4,0);
    }
    else 
    {
        //其他无效命令将-1发给客户端，客户端接收后输出相应内容
        ret=-1;
        send(pNode->newFd,&ret,4,0);
    }
    /* printf("Comand finish\n"); */
    return 0;
}
