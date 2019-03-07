#include"client.h"
//参数解析 域名解析
int get_parameter(char *hostname,char *buf)
{
	char *ipaddr = NULL;
	struct hostent *gethost;
	gethost = gethostbyname(hostname);
	char ip[100];
	if (NULL == gethost)
	{
		perror("");
		return -1;
	}
	memset(ip,0,sizeof(ip));
	ipaddr = (char *)inet_ntop(gethost->h_addrtype,gethost->h_addr_list[0],ip,sizeof(ip));
	strncpy(buf,ipaddr,sizeof(ip));
	return 0;
}
