#ifndef __HEAD_H__
#define __HEAD_H__
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<fcntl.h>
#include<grp.h>
#include<sys/select.h>
#include<sys/time.h>
#include<syslog.h>
#include<sys/mman.h>
#include<strings.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<errno.h>
#include<signal.h>
#include<pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <libgen.h>
#include <shadow.h>
#include <mysql/mysql.h>
#define ARGS_CHECK(argc,val){if(argc!=val){printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName){if(ret==retVal){perror(funcName);return -1;}}
#define VOID_ERROR_CHECK(ret,retVal,funcName){if(ret==retVal){perror(funcName);exit(-1);}}
#define THREAD_ERROR_CHECK(ret,funcName){if(ret!=0){printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define CHILD_THREAD_ERROR_CHECK(ret,funcName){if(ret!=0){printf("%s:%s\n",funcName,strerror(ret));return (void*)-1;}}

#define FP_CHECK(ret,retval,funcname)\
            do{if(ret==retval){perror(funcname);return -1;}}while(0)
#define PRINT_(pfd,num) do{for(int i=0;i<num;i++)fprintf(pfd,"─");}while(0)
#define PRINT_EMPTY(pfd,num) do{for(int i=0;i<num;i++)fprintf(pfd," ");}while(0)

typedef struct {
    char ip[16];
    char port[8];
    char threadNumber[8];
    char capacity[8];
    char server[20];
    char user[20];
    char password[20];
    char database[20];
}configuration_t,*pConfiguration_t;
int readConfigurationFile(const char*,pConfiguration_t);
/* int makeConnectLog(struct sockaddr_in client); */
/* int makeCmdLog(struct sockaddr_in client,char cmdName[]); */
int makeLog(char* username,char *ip,char *info,MYSQL *conn);
int logIn(char* username,int newFd,MYSQL *conn);
int signUp(char* username,int newFd,MYSQL *conn);

int mysqlQuery(char *query,char *buf,MYSQL *conn);
int mysqlInsert(char *query, MYSQL *conn);
int mysqlUpdate(char *query, MYSQL *conn);
int mysqlDelete(char *query, MYSQL *conn);
#endif
