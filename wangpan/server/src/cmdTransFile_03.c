#define _GNU_SOURCE 
#include "../include/ftpCmd.h"
int cmdGets(int newFd,int precode,char *filename,char *username,MYSQL *conn)//(相对客户端)下载
{
    //每次都要发火车头+车厢
    train_t train;
    int ret;
    /* char temp[1024]={0}; */
    char trueName[256]={0};
    char md5Str[36]={0};
    char query[256]={0}; 
    /* printf("name=%s\n",filename); */
    sprintf(query,"select md5sum from file where precode=%d and filename='%s' and owner='%s'",precode,filename,username);
    ret=mysqlQuery(query,md5Str,conn);
    send(newFd,&ret,4,0);
    struct stat buf;
    sprintf(trueName,"../file/%s",md5Str);
    /* printf("%s\n", temp); */
    int fd = open(trueName, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    /* train.dataLen = strlen(filename); */
    /* strcpy(train.buf, filename); */
    /* send(newFd, &train, train.dataLen + 4, 0); //文件名的长度为sizeof(int)=4,再加上文件名的内容(字符串)的长度 */
    fstat(fd, &buf); //获取打开文件的各种信息存入buf结构体。
    //发送文件大小
    train.dataLen = sizeof(buf.st_size);            //火车头
    memcpy(train.buf, &buf.st_size, train.dataLen); //
    send(newFd, &train, 4 + train.dataLen, 0);
    //============断点续传=================================

    /*接收断点信息，判断是否为断点*/
    off_t offset;
    recv(newFd,&train.dataLen,4,0);
    recv(newFd,train.buf,train.dataLen,0);
    memcpy(&offset,train.buf,train.dataLen);	
    if(offset)
    {
        lseek(fd,offset,SEEK_SET);
    }

    //===============END===================================
    //发送文件内容：依然是火车头+火车尾的形式
    int total = 0;
    if (buf.st_size < 104857600)
    {
        printf("little file,use send\n");
        while ((train.dataLen = read(fd, train.buf, sizeof(train.buf))))
        {
            ret = send(newFd, &train, 4 + train.dataLen, 0);
            if (-1 == ret)
            {
                return -1;
            }
            total = total + ret;
        }
        send(newFd, &train, 4, 0);
        /* printf("little train ret=%d\n", total); */
    }
    else
    {
        printf("big file, use sendfile\n");
        ret = sendfile(newFd, fd, NULL, buf.st_size);
        /* printf("sendfile ret=%d\n", ret); */
        ERROR_CHECK(ret, -1, "sendfile");
    }
    recvCycle(newFd,&ret,4);
    return 0;
}
int cmdPuts(int newFd,int precode,char *username,MYSQL *conn)//(对客户端来说)上传
{
    int fd,ret;
    int dataLen;
    char buf[1000] = {0};
    char trueName[1024]={0}; 
    char md5Str[36]={0};
    char query[256]={0};
    char filename[128]={0};
    char result[64]={0};
    off_t fileSize, downLoadSize = 0;
    recvCycle(newFd, &dataLen, 4);
    //收到-1说明对面输错了，返回-1
    if(-1==dataLen)
    {
        return -1;
    }
    //先接md5码,用md5码做真正的文件名
    recvCycle(newFd, md5Str, dataLen);
    sprintf(trueName,"../file/%s",md5Str);
    //再接文件名，用于数据库操作
    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd,&filename,dataLen);
    //先判断当前目录是否有同名文件，有则不让上传
    bzero(query,sizeof(query));
    sprintf(query,"select code from file where precode=%d and filename='%s' and owner='%s'",precode,filename,username);
    ret=mysqlQuery(query,buf,conn);
    //找到，将-1发给客户端，没找到发0
    if(0==ret)
    {
        ret=-1;
        send(newFd,&ret,4,0);
        return -1;
    }else{
        ret=0;
        send(newFd,&ret,4,0);
    }
    //若数据库中已有该md5码，说明本地已有该文件
    //则只需增加数据库数据，无需再次上传
    bzero(query,sizeof(query));
    bzero(buf,sizeof(buf));
    sprintf(query,"select fileSize from file where md5sum='%s'",md5Str);
    ret=mysqlQuery(query,result,conn);
    //若没找到才需要真正的上传
    send(newFd,&ret,4,0);
    if(-1==ret)
    {
        fd = open(trueName, O_CREAT | O_RDWR, 0666); //创建一个相同名字的文件
        ERROR_CHECK(fd, -1, "open");
        //============断点续传  1=================================
        /*检查文件大小，判断是否断点文件*/
        struct stat st;
        bzero(&st,sizeof(struct stat));	
        off_t offset;
        fstat(fd,&st);
        offset=st.st_size;
        if(offset)
        {
            lseek(fd,offset,SEEK_SET);
        }
        //===============1 END===================================
        //2接收文件大小
        /*修改 downloadSize初值*/
        downLoadSize=offset;

        recvCycle(newFd, &dataLen, 4);
        recvCycle(newFd, &fileSize, dataLen);
        /* struct timeval start, end; */
        /* gettimeofday(&start, NULL); */

        //============断点续传  2=================================
        /*发送断点信息*/
        train_t train;
        bzero(&train,sizeof(train_t));
        train.dataLen=sizeof(offset);
        memcpy(train.buf,&offset,sizeof(offset));
        send(newFd,&train,train.dataLen+4,0);

        //===============2  END===================================



        //3.接收文件内容

        if (fileSize < 104857600)
        {
            printf("little file, use recv\n");
            /* printf("little train\n"); */
            while (1)
            {
                recvCycle(newFd, &dataLen, 4); //接火车头
                if (dataLen > 0)
                {
                    ret=recvCycle(newFd, buf, dataLen);
                    if(-1==ret)
                    {
                        printf("Client disconnect\n");
                        return -1;
                    }
                    write(fd, buf, dataLen);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            printf("big file, use splice\n");
            int fds[2];
            pipe(fds);
            while (downLoadSize < fileSize)
            {
                ret = splice(newFd, NULL, fds[1], NULL, 65536, SPLICE_F_MOVE | SPLICE_F_MORE);
                ERROR_CHECK(ret, -1, "splice");
                if(0==ret)
                {
                    printf("Client disconnect\n");
                    return -1;
                }
                splice(fds[0], NULL, fd, NULL, ret, SPLICE_F_MOVE | SPLICE_F_MORE);
                downLoadSize += ret;
            }
            /* printf("downLoadSize=%d\n", (int)downLoadSize); */
        }
        bzero(query,sizeof(query));
        sprintf(query,"insert into file(precode,filename,owner,md5sum,filesize,type) values(%d,'%s','%s','%s',%ld,'-')",precode,filename,username,md5Str,fileSize);
        mysqlInsert(query,conn);      
        /* gettimeofday(&end, NULL); */
        /* printf("use time=%ld\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec); */
        close(fd);
    }
    else{
        fileSize=atol(result);
        bzero(query,sizeof(query));
        sprintf(query,"insert into file(precode,filename,owner,md5sum,filesize,type) values(%d,'%s','%s','%s',%ld,'-')",precode,filename,username,md5Str,fileSize);
        mysqlInsert(query,conn);      
    }
    return 0;
}
