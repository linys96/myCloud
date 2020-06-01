#include "../include/ftpCmd.h"
int cmdPwd(int precode,int newfd,MYSQL *conn)
{
    train_t train;
    bzero(&train,sizeof(train_t));
    char buf[256]={0};
    char query[256]={0};
    char result[256]={0};
    char tmp[256]={0};
    int prePrecode=precode;
    if(0==precode)
    {
        strcpy(train.buf,"~");
    }
    else
    {
        while(prePrecode)
        {
            bzero(query,sizeof(query));
            bzero(buf,sizeof(buf));
            sprintf(query,"select filename from file where code=%d",prePrecode);
            mysqlQuery(query,buf,conn);
            if(0==strcmp(result,""))
            {
                strcpy(result,buf);
            }
            else
            {
                bzero(tmp,sizeof(tmp));
                strcpy(tmp,result);
                sprintf(result,"%s/%s",buf,tmp);
            }
            bzero(query,sizeof(query));
            bzero(buf,sizeof(buf));
            sprintf(query,"select precode from file where code=%d",prePrecode);
            mysqlQuery(query,buf,conn);
            prePrecode=atoi(buf);
        }
        sprintf(train.buf,"~/%s",result);
    }
    train.dataLen=strlen(train.buf);
    send(newfd,&train,train.dataLen+4,0);
    return 0;
}

