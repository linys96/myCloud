#include "../include/ftpCmd.h"

int cmdMkdir(int precode,char *name,char *username,int newFd,MYSQL *conn)
{
    char *temp=NULL;
    char query[256]={0};
    char tmp[128]={"select code from file where precode="};
    char buf[16]={0};
    int ret;
    train_t train;
    temp=strtok(name," ");
    while(temp)
    {    
        bzero(query,sizeof(query));
        sprintf(query,"%s%d and filename='%s'",tmp,precode,temp);
        ret=mysqlQuery(query,buf,conn);
        if(0==ret)
        {
            ret=-1;
            send(newFd,&ret,4,0);
            bzero(train.buf,sizeof(train.buf));
            strcpy(train.buf,temp);
            train.dataLen=strlen(train.buf);
            send(newFd,&train,train.dataLen+4,0);
            return -1;
        }
        bzero(query,sizeof(query));
        sprintf(query,"insert into file(precode,filename,owner,filesize,type) values(%d,'%s','%s',4096,'d')",precode,temp,username);
        mysqlInsert(query,conn);        
        temp=strtok(NULL," ");
    }
    ret=0;
    send(newFd,&ret,4,0);
    return 0;
}

