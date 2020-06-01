#define _GNU_SOURCE
#include "../include/clientFtpCmd.h"
#include "../include/md5.h"
#define READ_DATA_SIZE  1024
#define MD5_SIZE        16
#define MD5_STR_LEN     (MD5_SIZE * 2)
//计算文件md5码

int Compute_file_md5(const char *file_path, char *md5_str)
{
    int i;
    int fd;
    int ret;
    unsigned char data[READ_DATA_SIZE];
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;

    fd = open(file_path, O_RDONLY);
    if (-1 == fd)
    {
        /* perror("open"); */
        return -1;
    }

    // init md5
    MD5Init(&md5);

    while (1)
    {
        ret = read(fd, data, READ_DATA_SIZE);
        if (-1 == ret)
        {
            perror("read");
            close(fd);
            return -1;
        }

        MD5Update(&md5, data, ret);

        if (0 == ret || ret < READ_DATA_SIZE)
        {
            break;
        }
    }

    close(fd);

    MD5Final(&md5, md5_value);

    // convert md5 value to md5 string
    for(i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
    }

    return 0;
}

int cmdGets(int newFd,char *filename)//下载
{
    //1.先接文件名
    int fd;
    int dataLen;
    char buf[1000] = {0};
    recvCycle(newFd, &dataLen, 4);
    //接受到-1说明文件名打错了
    if(-1==dataLen)
    {
        printf("gets error: %s: No such file\n",buf);
        return -1;
    }
    fd = open(filename, O_CREAT | O_RDWR, 0666); //创建一个相同名字的文件
    ERROR_CHECK(fd, -1, "open");
    //============断点续传--1--=================================
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
    off_t fileSize, downLoadSize = 0;
    /*修改 downloadSize初值*/
    downLoadSize=offset;
    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd, &fileSize, dataLen);
    /* struct timeval start, end; */
    /* gettimeofday(&start, NULL); */

    //============断点续传--2--=================================
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
        /* printf("little train\n"); */
        while (1)
        {
            recvCycle(newFd, &dataLen, 4); //接火车头
            if (dataLen > 0)
            {
                recvCycle(newFd, buf, dataLen);
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
        int ret;
        int fds[2];
        off_t slice=fileSize/1000;
        off_t lastDownLoadSize=downLoadSize;
        pipe(fds);
        while (downLoadSize < fileSize)
        {
            ret = splice(newFd, NULL, fds[1], NULL, 65536, SPLICE_F_MOVE | SPLICE_F_MORE);
            ERROR_CHECK(ret, -1, "splice");
            if(0==ret)
            {
                printf("Receive Error\n");
                return -1;
            }
            splice(fds[0], NULL, fd, NULL, ret, SPLICE_F_MOVE | SPLICE_F_MORE);
            downLoadSize += ret;
            if(downLoadSize-lastDownLoadSize>slice)
            {
                printf("%5.2f%s\r",(float)downLoadSize/fileSize*100,"%");
                fflush(stdout);
                lastDownLoadSize=downLoadSize;
            }
        }
        /* printf("downLoadSize=%d\n", (int)downLoadSize); */
    }
    /* gettimeofday(&end, NULL); */
    send(newFd,&dataLen,4,0);
    printf("gets successs\n");
    /* printf("use time=%ld\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec); */
    close(fd);
    return 0;
}
int cmdPuts(int newFd,char *filename)//上传
{
    //计算文件md5码
    int ret;
    char md5_str[MD5_STR_LEN + 1]={0};
    train_t train;
    ret=Compute_file_md5(filename,md5_str);
    if(-1==ret)
    {
        send(newFd,&ret,4,0);
        printf("puts error: %s: No such file\n",filename);
        return -1;
    }
    printf("md5=%s\n",md5_str);
    //先发送md5码
    bzero(train.buf,sizeof(train.buf));
    strcpy(train.buf,md5_str);
    train.dataLen = strlen(train.buf);
    send(newFd, &train, train.dataLen + 4, 0); 
    bzero(train.buf,sizeof(train.buf));
    strcpy(train.buf, filename);
    train.dataLen = strlen(filename);
    send(newFd, &train, train.dataLen + 4, 0); //文件名的长度为sizeof(int)=4,再加上文件名的内容(字符串)的长度
    //每次都要发火车头+车厢
    recvCycle(newFd,&ret,4);
    //接到-1说明目录下已有同名文件
    if(-1==ret)
    {
        printf("puts error: %s: File exists\n",filename);
        return -1;
    }
    recvCycle(newFd,&ret,4);
    if(-1==ret)
    {
        int fd = open(filename, O_RDWR);
        ERROR_CHECK(fd, -1, "open");
        struct stat buf;                           //结构体保存了文件的各种信息

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
            /* printf("little begin\n"); */
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
            ret = sendfile(newFd, fd, NULL, buf.st_size);
            /* printf("sendfile ret=%d\n", ret); */
            ERROR_CHECK(ret, -1, "sendfile");
        }
    }
    printf("puts success\n");
    return 0;
}
