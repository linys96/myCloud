#ifndef __FTPCMD_H__
#define __FTPCMD_H__
#include "head.h"

int clientVerify(int socketfd);
int clientFtpCmd(int sockfd,train_t *pTrain);
int cmdGets(int,char*);
int cmdPuts(int,char*);
int cmdPwd(int,char*);
#endif
