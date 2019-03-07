#include"server.h"
int sqlite_func(char *temper)//存入数据库
{
	sqlite3 *db;
	char *errmsg = 0;
	int ret;
	char ID_buf[20];
	char temp_buf[20];
	char data_buf[50];
    char *str[3];
	int i = 0;
	char *sql = "";
    char buf[1024];
	char *delim="|";
	char *p=strtok(temper,delim);
	if(NULL == p)
	{
		printf("strtok fail!\n");
		return -1;
	}
	while(p != NULL)
	{
		printf("%s\n",p);
        str[i] = p;
		p = strtok(NULL,delim);
        i = i+1;
        if(i>=3)
            i=0;
	}
    printf("%s %s %s",str[0],str[1],str[2]);
	ret = sqlite3_open("./debug/server.db",&db);
	if(ret != SQLITE_OK)
	{
		printf("open sqlite fail:%s",errmsg);
		return -1;
	}
	sql = "create table if not exists server(id TEXT, temper REAL,data TEXT);";
	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("create table fail:%s",errmsg);
		return -1;
	}
    printf("here\n");
	sprintf(buf,"insert into server values('%s','%s','%s');",str[0],str[1],str[2]);
    printf("%s\n",buf);
	ret = sqlite3_exec(db,(char *)&buf,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("insert values fail:%s",errmsg);
		return -1;
	}
    sqlite3_close(db);
	return 0;
}
