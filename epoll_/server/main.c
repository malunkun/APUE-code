/*************************************************************************
    > File Name: main.c
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com>
    > Created Time: 2019年03月23日 星期六 16时57分19秒
 ************************************************************************/
#include"server.h"
int main(int argc,char *argv[])
{
	int listen_fd;
	char msg[BUFFSIZE];
	listen_fd = socket_bind(msg);
	struct sockaddr_in addr;
	if(listen_fd < 0)
	{
		printf("%s",msg);
		return -1;
	}
	printf("成功初始化socket，开启监听!\n");
	
	return 0;
}
