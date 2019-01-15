/*************************************************************************
    > File Name: client.c
    > Author: malunkun
    > Mail: malunkun<209446860qq.com> 
    > Created Time: 2019年01月15日 星期二 14时13分53秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>  
#include<sys/socket.h>
#include<signal.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int optret;
	int ipaddr;
	int port_set;
	while((optret = getopt(argc,argv,"i:p:")) != -1)
	{
		switch(optret)
		{
			case 'i':
				printf("option = i,the ip will be set=%s\n",optarg);
				break;
			case 'p':
				printf("option = p,the port will be set=%s\n",optarg);
				break;
		}
	}
	if (socket(AF_INET,SOCK_STREAM,0) < 0)
	{
		perror("socket fail:");
		return -1;
	}
	printf("socket successful!\n");

	return 0;
}

