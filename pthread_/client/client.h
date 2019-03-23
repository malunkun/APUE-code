/*************************************************************************
    > File Name: client.c
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com> 
    > Created Time: 2019年01月15日 星期二 14时13分53秒
 ************************************************************************/

#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>  
#include<sys/socket.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include <sys/types.h>
#include<arpa/inet.h>
#include<time.h>

#define BUFFSIZE 1024
#define MALLOCSIZE 100
#define ID  "Ras_111"
#define SYSLOGFILE "./debug/client.log"
#define SYSERRFILE "./debug/clienterr.log"

int my_syslog(int *sys_fd,int *syserr_fd);//日志系统
int get_parameter(char *hostname,char *buf);//参数解析
int connect_server (char *ip,int port);//连接服务器
int get_temper(float *temp);//获取温度
