#include "../include/head.h"
int recvCycle(int fd,void *buf,off_t recvLen)
{
    char *p=(char*)buf;
    int ret;
    off_t completedSize=0;
    while(completedSize<recvLen)
    {
        ret=recv(fd,p+completedSize,recvLen-completedSize,0);
        if(0==ret)
        {
            return -1;
        }
        completedSize+=ret;
    }
    return 0;
}

