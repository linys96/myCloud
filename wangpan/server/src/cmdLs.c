#include "../include/ftpCmd.h"

int cmdLs(int newFd,int precode,char *username,MYSQL *conn)
{

    train_t train;
    char query[256]={"select type,filename,filesize from file where owner="};
    sprintf(query,"%s'%s' and precode=%d",query,username,precode);
    /* puts(query); */
    int t;
    char tmp[256]={0};
    MYSQL_RES *res;
    MYSQL_ROW row;
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);
        row = mysql_fetch_row(res);
        if(row == NULL)
        {
            /* printf("Didn't find data\n"); */
            return -1;
        }else{
            do{
                bzero(train.buf,sizeof(train.buf));
                sprintf(train.buf,"%4s",row[0]);
                for(t=1;t<mysql_num_fields(res);t++)
                {
                    bzero(tmp,sizeof(tmp));
                    sprintf(tmp," %12s",row[t]);
                    strcat(train.buf,tmp);
                }
                train.dataLen=strlen(train.buf); 
                send(newFd,&train,train.dataLen+4,0);
            }while((row=mysql_fetch_row(res))!=NULL); 
        }
        mysql_free_result(res);
    }
    train.dataLen=0;
    send(newFd,&train,4,0);    
	return 0;
}
