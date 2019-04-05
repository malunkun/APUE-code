#include "epoll.h"
int epoll_set(int lieten_fd,struct epoll_event *event,char *msg)
{
	int ret = -1;
	int epfd = -1;
	int event_num;
	epfd = epoll_create(FDSIZE);
	if(epfd < 0)
	{
		sprintf(msg,"创建epoll失败!\n",strerror(errno));
		return -1;
	}
	event.events = EPOLLIN|EPOLLET;
	event.data.fd= listen_fd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,event);
	if(ret < 0)
	{
		sprintf(msg,"epoll_ctl fail:%s\n",strerror(errno));
		return -1;
	}	
	return epfd;
}

int handle_events(int epfd,int listenfd,struct sockaddr_in *addr,socklen_t *addrlen,int eventnum,struct epoll_event *event,char *sendbuf)
{
	int fd;
	int ret = -1;
	for(int i=0;i<eventnum;i++)
	{
		fd = event[i].date.fd;
		if((fd&EPOLLERR)||(fd&EPOLLHUP))
		{
			printf("有挂断或出错\n");
			epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event);
			close(fd);
		}
		else if(fd = listenfd)
		{
			int acceptfd = accept(listenfd,addr,addrlen);
			if(accept < 0)
			{
				printf("accept fail!%s\n",strerror(errno));
				epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event);
				close(fd);
			}
			ret = read(acceptfd,sendbuf,BUFFSIZE);
			if(ret < 0)
			{
				printf("read fail%s\n",strerror(errno));
				return -1;
			}
			if(ret = 0)
			{
				printf("连接断开\n");
				close(fd);
				break;
			}
			ret = write(acceptfd,sendbuf,BUFFSIZE);
			if(ret < 0)
			{
				printf("write fail\n");
				return -1;
			}
			epoll_ctl(epfd,EPOLL_CTL_ADD,acceptfd,event);
		}
		else
		{
			printf("已连接fd%d\n",fd);
			ret = read(fd,sendbuf,BUFFSIZE);
			if(ret < 0)
			{
				printf("read fail%s\n",strerror(errno));
				return -1;
			}
			if(ret = 0)
			{
				printf("连接断开\n");
				close(fd);
				break;
			}
			ret = write(fd,sendbuf,BUFFSIZE);
			if(ret < 0)
			{
				printf("write fail\n");
				return -1;
			}
		}
	}
	return 0;
}
