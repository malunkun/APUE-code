/*************************************************************************
    > File Name: client.h
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com>
    > Created Time: 2019年03月23日 星期六 16时55分43秒
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
#include<sys/types.h>
#include<arpa/inet.h>
#include<time.h>

#define SYSLOG "./log/client.log"
#define ID "Ras_pi 001"
#define TPATH "/home/bus/w1/devices/" 

FILE *my_syslog(char *buf);//开启日志
void my_syslog_close(FILE *fp);//关闭日志
int get_parameter(int argc,char **argv,int *port,char *ip);//两个参数，解析IP及端口
int get_ip(char *hostname,char *buf);//解析IP
int connect_server (char *ip,int port,FILE *fp);//连接服务器
int get_temper(float *temp,FILE *fp);//读取温度
void my_time(char *buf);
