#define _GNU_SOURCE
#include "../include/clientFtpCmd.h"
#include "../include/color.h"
int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,3);
    //与服务器端建立连接
    int sockfd; 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    int ret,choice,flag=0;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
    serveraddr.sin_port=htons(atoi(argv[2]));
    ret=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
    ERROR_CHECK(ret,-1,"connect");
    printf("Connection success\n");
    //登录，验证账户密码
    while(1)
    {
        flag=0;
        printf(L_BLUE"===========>>\n" NONE);
        printf("Enter 1 to log in\n");
        printf("Enter 2 to sign up\n");
        printf("Enter 0 to exit\n");
        printf(L_BLUE"===========>>\n" NONE);
        scanf("%d",&choice);
        send(sockfd,&choice,4,0);
        switch(choice)
        {
        case 0: return 0;
        case 1:
                do{ 
                    ret=logIn(sockfd);
                }while(ret);
                printf("Log in success\n");
                flag=1;
                break;
        case 2:
                do{ 
                    ret=signUp(sockfd);
                }while(ret);
                printf("Sign up success\n");
                flag=1;
                break;
        default:
                printf("Wrong input, please enter again\n");
        }
        if(flag)
        {
            break;
        }
    }
    //epoll监听标准输入和服务器端的输入
    int epid,readyFdNumber;
    int i;
    epid=epoll_create(1);
    ERROR_CHECK(epid,-1,"epoll_create");
    struct epoll_event evs[2];
    epollInAdd(epid,STDIN_FILENO); 
    epollInAdd(epid,sockfd);
    train_t train;
    recvCycle(sockfd,&train.dataLen,sizeof(int));
    recvCycle(sockfd,train.buf,train.dataLen);
    printf(PURPLE"%s" NONE,train.buf);
    fflush(stdout);
    while(1){
        /* printf("test\n"); */
        readyFdNumber=epoll_wait(epid,evs,2,-1);
        for(i=0;i<readyFdNumber;i++)
        {
            //标准输入可读
            if(evs[i].data.fd==STDIN_FILENO)
            {
                bzero(&train,sizeof(train_t));
                read(STDIN_FILENO,train.buf,sizeof(train.buf));
                train.buf[strlen(train.buf)-1]=0;//去掉末尾的\n
                train.dataLen=strlen(train.buf);
                send(sockfd,&train,train.dataLen+4,0);
            }
            //服务端可读
            if(evs[i].data.fd==sockfd)
            {
                //因为客户端给服务端发命令后服务端会将命令回发一份
                //所以先接命令，将命令存入小火车
                bzero(train.buf,sizeof(train.buf));
                recvCycle(sockfd,&train.dataLen,4);
                //如果接到的是-1，说明输入的命令无效
                if(-1==train.dataLen)
                {
                    printf("Invalid operation, enter help to get help\n");
                    bzero(train.buf,sizeof(train.buf));
                    recvCycle(sockfd,&train.dataLen,sizeof(int));
                    recvCycle(sockfd,train.buf,train.dataLen);
                    printf(PURPLE"%s" NONE,train.buf);
                    fflush(stdout);
                    break;
                }
                recvCycle(sockfd,train.buf,train.dataLen);
                clientFtpCmd(sockfd,&train);

                bzero(train.buf,sizeof(train.buf));
                recvCycle(sockfd,&train.dataLen,sizeof(int));
                recvCycle(sockfd,train.buf,train.dataLen);
                printf(PURPLE"%s" NONE,train.buf);
                fflush(stdout);
            }
        }
    }

    close(sockfd);
    return 0;

}

