#include "../include/ftpCmd.h"
//登录
int logIn(char *username,int newFd,MYSQL *conn)
{
    train_t train;
    int ret,dataLen;
    char tmp[128]={0};
    char ciphertext[128]={0};
    /* char test[50][256]={0}; */
    recvCycle(newFd,&dataLen,4);
    recvCycle(newFd,tmp,dataLen);
    char query[128]="select salt from user where name='";
    sprintf(query,"%s%s%s",query,tmp,"'");
    bzero(train.buf,sizeof(train.buf));
    ret=mysqlQuery(query,train.buf,conn);
    /* printf("train buf=%s\n",train.buf); */
    if(-1==ret)
    {
        send(newFd,&ret,4,0);
        return -1;
    }
    train.dataLen=strlen(train.buf);
    send(newFd,&train,train.dataLen+4,0);
    
    bzero(train.buf,sizeof(train.buf));
    recvCycle(newFd,&train.dataLen,4);
    recvCycle(newFd,train.buf,train.dataLen);

    bzero(query,sizeof(query));
    sprintf(query,"select ciphertext from user where name='%s'",tmp);
    mysqlQuery(query,ciphertext,conn);
    ret=strcmp(ciphertext,train.buf);
    send(newFd,&ret,4,0);
    if(ret)
    {
        return -1;
    }
    strcpy(username,tmp);
    return 0;
}
//注册
int signUp(char *username,int newFd,MYSQL *conn)
{
    int ret,dataLen;
    char salt[12]={0};
    char tmp[64]={0};
    char ciphertext[128]={0};
    recvCycle(newFd,&dataLen,4);
    recvCycle(newFd,tmp,dataLen);
    char query[256]="select salt from user where name='";
    sprintf(query,"%s%s%s",query,tmp,"'");
    ret=mysqlQuery(query,tmp,conn);
    //用户名已存在，返回0；
    
    send(newFd,&ret,4,0);
    if(0==ret)
    {
        return -1;
    }
    recvCycle(newFd,&dataLen,4);
    recvCycle(newFd,salt,dataLen);
    
    recvCycle(newFd,&dataLen,4);
    recvCycle(newFd,ciphertext,dataLen);
    bzero(query,sizeof(query));
    sprintf(query,"insert into user(name,salt,ciphertext) values('%s','%s','%s')",tmp,salt,ciphertext);
    ret=mysqlInsert(query,conn);
    strcpy(username,tmp);
    return 0;
}
