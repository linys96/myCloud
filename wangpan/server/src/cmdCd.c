#include "../include/ftpCmd.h"

int cmdCd(int *precode,char *name,int newfd,MYSQL *conn)
{
    /* printf("name=%s\n",name); */
    if(0==strcmp(name,"")||0==strcmp(name,"~"))
    {
        /* printf("in\n"); */
        *precode=0;//回到家目录
        send(newfd,precode,4,0);
        return 0;
    }
    train_t train;
    bzero(&train,sizeof(train));

    char change[256]={0};
    strcpy(change,name);
    int ret;
    int prePrecode=*precode;
    //开始获得每一个字符串
    char *token=strtok(change,"/");
    char tmp[]={"select code from file where type='d' and filename='"};
    char query[256]={0};
    char result[32]={0};
    if(NULL==token)
    {
        if(strcmp(name,"..")==0)
        {
            if(*precode==0)
            {
                send(newfd,precode,4,0);
            }
            else
            {
                bzero(query,sizeof(query));
                bzero(result,sizeof(result));
                sprintf(query,"select precode from file where code=%d",*precode);
                ret=mysqlQuery(query,result,conn);
                *precode=atoi(result);
                send(newfd,&ret,4,0);
            }
            return 0;
        }
        sprintf(query,"%s%s' and precode=%d",tmp,name,*precode);
        ret=mysqlQuery(query,result,conn);
        send(newfd,&ret,4,0);
        if(ret)
        {
            return -1;
        }
        *precode=atoi(result);
        return 0; 
    }

    while(token)
    {
        if(strcmp(token,"..")==0)
        {
            if(prePrecode==0)
            {
                /* send(newfd,&precode,4,0); */
            }
            else
            {
                bzero(query,sizeof(query));
                bzero(result,sizeof(result));
                sprintf(query,"select precode from file where code=%d",prePrecode);
                ret=mysqlQuery(query,result,conn);
                prePrecode=atoi(result);
                /* send(newfd,&ret,4,0); */
            }
        }
        else
        {
            bzero(query,sizeof(query));
            bzero(result,sizeof(result));
            sprintf(query,"%s%s' and precode=%d",tmp,token,prePrecode);
            ret=mysqlQuery(query,result,conn);
            if(ret)
            {
                send(newfd,&ret,4,0);
                return -1;
            }
            prePrecode=atoi(result);
        }
        token=strtok(NULL,"/");
    }
    ret=0;
    send(newfd,&ret,4,0);
    *precode=prePrecode;
    return 0;
}
