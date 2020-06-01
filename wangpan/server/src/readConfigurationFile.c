#include "../include/head.h"
#define MAX_BUF_LEN 1024
#define MAX_KEY_LEN 64
#define MAX_VAL_LEN 256

/*配置文件信息为 key=value*/
int readConfigurationFile(const char *filepath,pConfiguration_t pConfig)
{
    FILE *fp=fopen(filepath,"r");
    ERROR_CHECK(fp,NULL,"fopen");

    char buf[1024]={0};
    int commentFlag=0;
    char key[MAX_KEY_LEN]={0},value[MAX_VAL_LEN]={0};
    int i, keyLen=0, ValueLen=0,keyValueFlag=0;
    while(bzero(buf,1024),fgets(buf,1024,fp))
    {
        int bufLen=strlen(buf);
        // ignore and skip the line with first chracter '#', '=' or '/'
        if(bufLen<=2||buf[0]=='#'||buf[0]=='='||buf[0]=='/')
        {
            continue;
        }
        buf[bufLen-1]=0;//将末尾的换行去了
        // to skip comment text /* ... */
        if(buf[0]!='#'&&(buf[0]!='/'||buf[1]!='/'))
        {
            if(strstr(buf,"/*"))
            {
                commentFlag=1;
            }
            if(strstr(buf,"*/"))
            {
                commentFlag=0;
                continue;
            }
        }
        if(commentFlag)
        {
            continue;
        }

        keyValueFlag=0;
        keyLen=0;
        ValueLen=0;
        bzero(key,MAX_KEY_LEN);
        bzero(value,MAX_VAL_LEN);
        for(i=0;i<MAX_BUF_LEN;i++)
        {
            if(buf[i]==' ')
                continue;
            if(keyValueFlag==0&&buf[i]!='=')
            {
                if(keyLen>=MAX_KEY_LEN)
                    break;
                key[keyLen++]=buf[i];
                continue;
            }
            else if(buf[i]=='=')
            {
                keyValueFlag=1;
                continue;
            }
            if(ValueLen>=MAX_VAL_LEN||buf[i]=='#')
                break;
            value[ValueLen++]=buf[i];
        }
        if(strcmp(key,"")==0||strcmp(value,"")==0)
            continue;
        
        //将信息存入config结构体
        if(strcmp(key,"ip")==0)
        {
            strcpy(pConfig->ip,value);
        }
        else if(strcmp(key,"port")==0)
        {
            strcpy(pConfig->port,value);
        }
        else if(strcmp(key,"threadNumber")==0)
        {
            strcpy(pConfig->threadNumber,value);
        }
        else if(strcmp(key,"capacity")==0)
        {
            strcpy(pConfig->capacity,value);
        }
        else if(strcmp(key,"server")==0)
        {
            strcpy(pConfig->server,value);
        }
        else if(strcmp(key,"user")==0)
        {
            strcpy(pConfig->user,value);
        }
        else if(strcmp(key,"password")==0)
        {
            strcpy(pConfig->password,value);
        }
        else if(strcmp(key,"database")==0)
        {
            strcpy(pConfig->database,value);
        }
    }
    return 0;
}

