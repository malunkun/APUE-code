#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "sqlite3.h"

#define PORT 9005
#define BUFFSIZE 1024
#define SYSLOGFILE "./debug/server.log"
#define SYSERRFILE "./debug/servererr.log"

void *pthread_func(void *args);
int my_syslog(int *sys_fd,int *syserr_fd);//日志系统记录
void pthread_start(int fd);//开启线程
int sqlite_func(char *temper);//存入数据库

