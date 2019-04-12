#include"server.h"
int socket_bind(char *msg)
{
	int sockfd = -1;
	int ret = -1;
	struct sockaddr_in addr;
	socklen_t addrlen;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		sprintf(msg,"创建socket失败!%s\n",strerror(errno));
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret < 0)
	{
		sprintf(msg,"绑定IP端口失败!%s\n",strerror(errno));
		return -1;
	}
	ret = listen(sockfd,13);
	if(ret < 0)
	{
		sprintf(msg,"监听失败!%s\n",strerror(errno));
		return -1;
	}
	return sockfd;
}
