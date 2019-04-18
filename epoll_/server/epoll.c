/*************************************************************************
    > File Name: epoll.c
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com>
    > Created Time: 2019年04月12日 星期五 17时23分03秒
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9005
#define EPSIZE 1000
#define DOEPSIZE 200

int main(int argc,char argv[])
{
	int ret = -1;

	struct sockaddr_in addr;
	socklen_t len;
	int sockfd = -1;
	int listenfd = -1;
	char buf[1024];

	struct epoll_event ev,events[DOEPSIZE];
	int epfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	printf("sockfd:%d\n",sockfd);
	memset(&addr,0,sizeof(addr));
	addr.sin_family 	 = AF_INET;
	addr.sin_port   	 = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if (ret<0)
	{
		printf("bind fail:%s\n",strerror(errno) );
		return -1;
	}
	ret = listen(sockfd,13);
	if (ret < 0)
	{
		printf("listen fail:%s\n",strerror(errno));
		return -1;
	}

	epfd = epoll_create(EPSIZE);
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
	if (ret < 0)
	{
		printf("ctl fail!\n");
	}
	printf("ev.data.fd：%d\n",ev.data.fd);
	while(1)
	{
		printf("epoll start!\n");
		ret = epoll_wait(epfd,events,DOEPSIZE,-1);
		if(ret < 0)
		{
			printf("epoll failure: %s\n", strerror(errno));
			break;
		}
		else if(ret == 0)
		{	
			printf("epoll get timeout\n");
			continue;
		}
		printf("wait size:%d\n",ret);
		for(int i=0;i<ret;i++)
		{
			int fd = events[i].data.fd;
			if ( (event_array[i].events&EPOLLERR) || (event_array[i].events&EPOLLHUP) )
			{
				printf("epoll_wait get error on fd[%d]: %s\n",fd, strerror(errno));
				epoll_ctl(epollfd, EPOLL_CTL_DEL,fd, NULL);
				close(fd);
			}
			if (fd == sockfd)
			{
				printf("start accept!\n");
				listenfd = accept(sockfd,(struct sockaddr *)&addr,&len);
				if (listenfd < 0)
				{
					printf("accept fail:%s\n",strerror(errno));
					continue;
				}
				printf("客户端已连接\n");
				ev.data.fd = listenfd;
				ev.events  = EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
				printf("成功添加新连接!%d\n",listenfd);
			}
			else 
			{
				printf("start read and write!%d\n",fd);
				if (read(listenfd,buf,sizeof(buf)) <= 0)
				{
					printf("连接断开 or read出错:%s\n",strerror(errno));
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
					close(fd);
					continue;
				}
				if (write(fd,buf,sizeof(buf)) < 0)
				{
					printf("write fail:%s\n",strerror(errno));
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
					close(fd);
					continue;
				}
				printf("GET:%s\n",buf);
			}
		}		
	}
	close(epfd);
}
