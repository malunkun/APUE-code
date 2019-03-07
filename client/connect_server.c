#include"client.h"
//服务器连接
int connect_server (char *ip,int port)
{
	int sockfd = -1;
	struct sockaddr_in cli_addr;
	printf("the func connect_server ip = %s,port = %d\n",ip,port);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0)
	{
		perror("socket fail!");
		return -1;
	}
	printf("socket successfully!\n");
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family= AF_INET;
	cli_addr.sin_port = htons((uint16_t)(port));
	inet_aton(ip,&cli_addr.sin_addr);
	printf("ready to connect!\n");
	if ((connect(sockfd,(struct sockaddr *)&cli_addr,sizeof(cli_addr))) < 0)
	{
		perror("connect fail!");
		return -1;
	}
	return sockfd;
}
