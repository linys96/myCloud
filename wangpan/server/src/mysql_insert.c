#include "../include/ftpCmd.h"
int mysqlInsert(char *query, MYSQL *conn)
{
	int t;
    /* puts(query); */
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}
    else
    {
		/* printf("Insert success\n"); */
	}
	return 0;
}
