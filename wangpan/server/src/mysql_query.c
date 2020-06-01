#include "../include/ftpCmd.h"
//每次只能返回第一个查询结果
int mysqlQuery(char *query,char *buf,MYSQL *conn)
{
    int ret;
    MYSQL_RES *res;
    MYSQL_ROW row;
    /* puts(query); */
    ret=mysql_query(conn,query);
    /* printf("ret=%d\n",ret); */
    if(ret) 
    {
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }
    else
    {
        res=mysql_use_result(conn);
        row = mysql_fetch_row(res);
        if(row==NULL)
        {
            /* printf("Didn't find data\n"); */
            return -1;
        }
        else
        {
            strcpy(buf,row[0]);
            mysql_free_result(res);
        }
    }
    return 0;
}
