#include "../include/ftpCmd.h"
//删除文件
int cmdRemove(int precode,char *name,char *username,int newFd,MYSQL *conn)
{
    char query[256]={0};
    char md5sum[64]={0};
    char buf[64]={0};
    int ret,code;
    sprintf(query,"select code from file where filename='%s' and owner='%s' and precode=%d",name,username,precode);
    ret=mysqlQuery(query,buf,conn);
    //没找到
    if(ret)
    {
        send(newFd,&ret,4,0);
        return -1;
    }
    //找到了判断是目录还是文件
    code=atoi(buf);
    bzero(query,sizeof(query));
    sprintf(query,"select md5sum from file where code=%d",code);
    ret=mysqlQuery(query,md5sum,conn);
    if(ret)
    {
        printf("md5sum not found\n");
        return -1;
    }
    if(strcmp(md5sum,"directory"))
    {
        /* printf("file\n"); */
        bzero(query,sizeof(query));
        sprintf(query,"delete from file where code=%d",code);
        mysqlDelete(query,conn);
        bzero(query,sizeof(query));
        sprintf(query,"select code from file where md5sum='%s'",md5sum);
        ret=mysqlQuery(query,md5sum,conn);
        //没找到，就删除本地文件
        if(ret)
        {
            bzero(buf,sizeof(buf));
            sprintf(buf,"../file/%s",md5sum);
            remove(buf);
        }
    }
    else
    {
        /* ret=-2; */
        /* send(newFd,&ret,4,0); */
        /* int prePrecode=code; */
        bzero(query,sizeof(query));
        char temp[64]={0};
        sprintf(query,"select filename from file where precode=%d",code);
        ret=mysqlQuery(query,temp,conn);
        while(ret!=-1)
        {
            cmdRemove(code,temp,username,newFd,conn);
            bzero(temp,sizeof(temp));
            ret=mysqlQuery(query,temp,conn);
        }
        bzero(query,sizeof(query));
        sprintf(query,"delete from file where code=%d",code);
        mysqlDelete(query,conn);
        /* printf("directory\n"); */
    }
    ret=0;
	return 0;
}	
