#include "../include/head.h"

int tcpInit(int* sfd,char* ip,char* port)
{
    int ret;
    int sockfd;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(ip);
    serveraddr.sin_port=htons(atoi(port));
    int reuse=1;
    ret=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
    ERROR_CHECK(ret,-1,"bind");
    ret=listen(sockfd,10);
    ERROR_CHECK(ret,-1,"listen");
    *sfd=sockfd;
    return 0;
}

