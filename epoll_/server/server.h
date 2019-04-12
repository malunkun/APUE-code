#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>

#include "sqlite3.h"

#define PORT 9005 
#define FDSIZE 1000
#define EPOLLEVENTS 100
#define BUFFSIZE 1024
#define SYSLOG "./log/server.log"

int socket_bind(char *msg);//初始化socket，启用监听
int epoll_set(int listen_fd,struct sockaddr_in *addr,socklen_t *addrlen,struct epoll_event *event,char *msg);//epoll初始化
int handle_events(int epfd,int listenfd,int eventnum,struct epoll_event *event,char *sendbuf);
