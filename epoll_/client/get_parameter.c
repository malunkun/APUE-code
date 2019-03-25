#include"client.h"

int get_parameter(int argc,char **argv,int *port,char *ip)
{
	char *_argv[argc];
	int optret = 0;
	char ipArry[1024];
	for(int i=0;i<argc;i++)
		_argv[i] = *(argv+i);
	while((optret =getopt(argc,_argv,"i:p:"))!=-1)
	{
		switch(optret)
		{
			case 'i':
				if(get_ip(optarg,ipArry)<0)
					strncpy(ip,optarg,sizeof(ipArry));
				strncpy(ip,ipArry,sizeof(ipArry));
				break;
			case 'p':
				*port = atoi(optarg);
				break;
			default:
				printf("输入格式：./client -p [port] -i [ip or hostname]\n");
				break;
		}
	}
		if(*port==0||*ip==0)
			return -1;
		return 1;
}
int get_ip(char *hostname,char *buf)
{
	char *ipaddr = NULL;
	struct hostent *gethost;
	gethost = gethostbyname(hostname);
	char ip[100];
	if (NULL == gethost)
	{
		return -1;
	}
	memset(ip,0,sizeof(ip));
	ipaddr = (char *)inet_ntop(gethost->h_addrtype,gethost->h_addr_list[0],ip,sizeof(ip));
	strncpy(buf,ipaddr,sizeof(ip));
	return 0;
}

