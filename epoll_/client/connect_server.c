#include"client.h"
//服务器连接
int connect_server (char *ip,int port,FILE *fp)
{
	int sockfd = -1;
	struct sockaddr_in cli_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0)
	{
		fprintf(fp,"socket初始化失败:%s\n",strerror(errno));
		return -1;
	}
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family= AF_INET;
	cli_addr.sin_port = htons((uint16_t)(port));
	inet_aton(ip,&cli_addr.sin_addr);
	fprintf(fp,"准备连接%s\nport :%d\n",ip,port);
	if ((connect(sockfd,(struct sockaddr *)&cli_addr,sizeof(cli_addr))) < 0)
	{
		fprintf(fp,"连接到:%s 端口:%d 失败:%s\n",ip,port,strerror(errno));
		return -1;
	}
	return sockfd;
}
