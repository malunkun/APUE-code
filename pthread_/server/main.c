#include"server.h"
int go_stop = 0;
void hander(int num)
{
	go_stop = 1;
}
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
	signal(SIGQUIT,hander);
	mode_t mode = umask(0);
	if(mkdir("./debug/",0777)<0)
	{
		printf("mkdir fail!\n");
		return 0;
	}
	ret = my_syslog(&sys_fd,&syserr_fd);//日志系统
    daemon(1,1);//开启守护进程
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

    fflush(stdout);
    fflush(stderr);

	while(!go_stop)
	{
		 listen_fd = accept(sock_fd,(struct sockaddr *)&servaddr,(socklen_t *)&addrlen);
		 printf("accept successful!\n");
		 pthread_start(listen_fd);
         fflush(stdout);
         fflush(stderr);
	}
	close(sys_fd);
	close(syserr_fd);
	umask(mode);
	return 0;
}
