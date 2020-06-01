#define _XOPEN_SOURCE
#include "../include/head.h"
#include "../include/clientFtpCmd.h"
int logIn(int socketFd)
{
    train_t train;
    int dataLen;
    char salt[12]={0};
    char *password;
    
    bzero(&train,sizeof(train_t));
    printf("Please enter username:\n");
    scanf("%s",train.buf);
    train.dataLen=strlen(train.buf);
    send(socketFd,&train,train.dataLen+4,0);

    recvCycle(socketFd,&dataLen,4);
    if(-1==dataLen)
    {
        printf("Username not found\n");
        return -1;
    }

    recvCycle(socketFd,&salt,dataLen);
    bzero(&train,sizeof(train_t));
    password=getpass("Please enter password:");
    strcpy(train.buf,crypt(password,salt));
    train.dataLen=strlen(train.buf);
    send(socketFd,&train,train.dataLen+4,0);

    recvCycle(socketFd,&dataLen,4);
    if(dataLen)
    {
        printf("Incorrect password\n");
        return -1;
    }
    return 0;
}
int generateRandString(char *str,int len)
{
    int i,flag;
    srand(time(NULL));
    for(i = 0; i < len; i ++)
    {
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break;
        case 2:
            str[i] = rand()%10 + '0';
            break;
        }
    }
    return 0;
}
//注册
int signUp(int socketFd)
{
    train_t train;
    int dataLen;
    char salt[12]={"$6$"};
    char *password;
    
    printf("Please enter username:\n");
    bzero(&train,sizeof(train_t));
    scanf("%s",train.buf);
    train.dataLen=strlen(train.buf);
    send(socketFd,&train,train.dataLen+4,0);
    recvCycle(socketFd,&dataLen,4);
    if(0==dataLen)
    {
        printf("Username already exists\n");
        return -1;
    }
    
    generateRandString(salt+3,8);
    /* printf("salt=%s\n",salt); */
    bzero(&train,sizeof(train_t));
    train.dataLen=strlen(salt);
    strcpy(train.buf,salt);
    send(socketFd,&train,train.dataLen+4,0);

    bzero(&train,sizeof(train_t));
    password=getpass("Please enter password:");
    strcpy(train.buf,crypt(password,salt));
    train.dataLen=strlen(train.buf);
    send(socketFd,&train,train.dataLen+4,0);
    
    return 0;
}
