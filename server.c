#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "sqlite3.h"

#define PORT 9005
#define BUFFSIZE 1024
#define SYSLOGFILE "./server.log"
#define SYSERRFILE "./servererr.log"

void *pthread_func(void *args);
int my_syslog(int *sys_fd,int *syserr_fd)//日志系统记录
void pthread_start(int fd);//开启线程
int sqlite_func(char *temper);//存入数据库

int main(int argc,char *argv)
{
	int sock_fd;
	int listen_fd;
	int sys_fd = -1;
	int syserr_fd = -1;
	int ret = -1;
	struct sockaddr_in servaddr;
	socklen_t addrlen;
	int backlog = 13;
	ret = my_syslog(&sys_fd,&syserr_fd);
	if(ret < 0)
	{
		printf("open syslog fail!\n");
	}
	sock_fd = socket(AF_INET,SOCK_STREAM,0);

	if (sock_fd < 0)
	{
		perror("create socket fail!");
		return -1;
	}
	printf("create socket successful!\n");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock_fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		perror("bind fail!");
		return -2;
	}
	printf("bind successful!\n");
	if (listen(sock_fd,backlog)<0)
	{
		perror("listen fail!");
		return -3;
	}
	printf("listen successful!\n");

	while(1)
	{
		 listen_fd = accept(sock_fd,(struct sockaddr *)&servaddr,(socklen_t *)&addrlen);
		 printf("accept successful!\n");
		 pthread_start(listen_fd);
	}
	close(sys_fd);
	close(syserr_fd);
	return 0;
}

//日志系统
int my_syslog(int *sys_fd,int *syserr_fd)
{
	int fd;
	fd = open(SYSLOGFILE,O_CREAT|O_RDWR,0644);
	if(fd < 0)
	{
		perror("open syslog_file fail!\n");
		return -1;
	}
	*sys_fd = dup2(fd,STDOUT_FILENO);
	close(fd);
	lseek(*sys_fd,0,SEEK_END);
	fd = open(SYSERRFILE,O_CREAT|O_RDWR,0644);
	if(fd < 0)
	{
		perror("open syserrlog_file fail!");
		return -1;
	}
	*syserr_fd = dup2(fd,STDERR_FILENO);
	close(fd);
	lseek(*syserr_fd,0,SEEK_END);
	return 0;
}

void pthread_start(int fd)//////线程开启
{
	pthread_t tid;
	pthread_attr_t thread_attr;
	if (pthread_attr_init(&thread_attr))
	{
		perror("pthread_attr_init() fail!");
	}
	if (pthread_attr_setstacksize(&thread_attr,120*1024))
	{
		perror("pthread_attr_setstacksize() fail!");
	}
	if (pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED))
	{
		perror("pthread_attr_setdetachstate() fail!");
	}
	pthread_create(&tid,&thread_attr,pthread_func,(void *)fd);

cleanup:
	pthread_attr_destroy(&thread_attr);	
	printf("destroy attr successful!\n");
}

void *pthread_func(void *args)
{
	int plisten_fd = (int)args;
	char buf[BUFFSIZE];
	int sqlite_ret;
	int rd;
	while(1)
	{
		memset(buf,0,sizeof(buf));
		rd = read(plisten_fd,buf,sizeof(buf));
		if (rd <= 0)
		{
			perror("read fail!");
			break;
		}
		if(write(plisten_fd,buf,sizeof(buf)) < 0)
		{
			perror("write fail!");
			break;
		}
		printf("%s\n",buf);
		sqlite_ret = sqlite_func((char *)&buf);
	} 
		close(plisten_fd);
		return NULL;
}

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
	ret = sqlite3_open("./server.db",&db);
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
	return 0;
}
