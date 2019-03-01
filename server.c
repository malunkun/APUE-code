#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 9005
#define BUFFSIZE 1024
void *pthread_func(void *args);
void pthread_start(int fd);

int main(int argc,char *argv)
{
	int sock_fd;
	int listen_fd;
	struct sockaddr_in servaddr;
	socklen_t addrlen;
	int backlog = 13;
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
	return 0;
}


void pthread_start(int fd)//////
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
	} 
		close(plisten_fd);
		return NULL;
}

