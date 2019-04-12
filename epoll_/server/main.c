/*************************************************************************
    > File Name: main.c
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com>
    > Created Time: 2019年03月23日 星期六 16时57分19秒
 ************************************************************************/
#include"server.h"
int main(int argc,char *argv[])
{

	int ret = -1;
	int listen_fd = -1;
	char sendbuf[BUFFSIZE];
	char msg[BUFFSIZE];
	memset(&msg,0,sizeof(msg));

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct epoll_event events[EPOLLEVENTS];
	int epfd = -1;
	int eventnum = 0;

	memset(&addr,0,sizeof(addr));
	listen_fd = socket_bind(msg);
	if(listen_fd < 0)
	{
		printf("%s\n",msg);
		return -1;
	}
	printf("成功初始化socket，开启监听,fd:%d!\n",listen_fd);

	epfd = epoll_set(listen_fd,&addr,&addrlen,events,msg);
	if(epfd < 0)
	{
		printf("%s\n",msg);
		return -1;
	}
	printf("fd:%d\n",events[0].data.fd);

	while(1)
	{
		printf("epoll\n");
		eventnum = epoll_wait(epfd,events,EPOLLEVENTS,-1);
		printf("eventnum:%d\n",eventnum);
		if(eventnum == 0)
		{
			printf("超时%s\n",strerror(errno));
		}

		ret = handle_events(epfd,listen_fd,&addr,&addrlen,eventnum,events,sendbuf);
	}
	close(epfd);

	return 0;
}
