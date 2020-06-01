#ifndef __FTPCMD_H__
#define __FTPCMD_H__
#include "factory.h"
int cmdPrintPath(int precode,char *username,int newfd,MYSQL *conn);
int ftpCmd(pNode_t ,train_t *,MYSQL *);
int cmdGets(int newFd,int precode,char *filename,char *username,MYSQL *conn);
int cmdPuts(int newFd,int precode,char *username,MYSQL *conn);
int cmdRemove(int precode,char *name,char *username,int newfd,MYSQL *conn);
int cmdLs(int,int,char*,MYSQL*);
int cmdPwd(int precode,int newfd,MYSQL *conn);
int cmdCd(int *precode,char *name,int newfd,MYSQL *conn);
int cmdMkdir(int precode,char *name,char *username,int newFd,MYSQL *conn);
#endif
