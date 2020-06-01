#include "../include/head.h"

int makeLog(char* username,char *ip,char *info,MYSQL *conn)
{
    char query[1024]={"insert into log(username,ip,information)"};
    sprintf(query,"%s values('%s','%s','%s')",query,username,ip,info);
    int ret=mysqlInsert(query,conn);
    if(-1==ret)
    {
        printf("make log error\n");
        return -1;
    }
    return 0;
}

