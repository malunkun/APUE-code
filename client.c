/*************************************************************************
    > File Name: client.c
    > Author: malunkun
    > Mail: malunkun<209446860qq.com> 
    > Created Time: 2019年01月15日 星期二 14时13分53秒
 ************************************************************************/

#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>  
#include<sys/socket.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>

#define BUFFSIZE 1024
#define PORT     8848
#define IP       "127.0.0.1"

int connect_val = 0;
int get_temper_val = 0;

char *get_parameter(char *ipname);//获取参数，解析域名
int connect_server (char *ip,char *port);//连接服务器
int get_temper(float *temp);//获取温度

int main(int argc,char *argv[])
{
	int optret;
	char *ipaddr;
	char *port;
	char *hostname;
	int cli_sockfd;
	int temper;
	float buf;
	/*
	while((optret = getopt(argc,argv,"i:p:")) != -1)
	{
		switch(optret)
		{
			case 'i':
				printf("option = i,the ip will be set=%s\n",optarg);
				ipaddr = optarg;
				break;
			case 'h':
				printf("option = h,the hostname will be set=%s\n",optarg);
				hostname = optarg;
				break;
			case 'p':
				printf("option = p,the port will be set=%s\n",optarg);
				port = optarg;
				break;
			default:
				printf("please put into as:[-i]ip [-p]port or [-h]hostname [-p]port\n");
				break;
		}
	} 
	//ipaddr = get_parameter(hostname);
	//if (NULL == ipaddr)
	//{
	//	perror("get_parameter() fail!");
	//	return -1;
	//}
	printf("get_parameter() successful!\n");
	*/
	while(!connect_val)
	{
		cli_sockfd = connect_server (ipaddr,port);
		while(!get_temper_val && connect_val)
		{
			if (get_temper(&buf) < 0)
			{
				perror("get temperature fail!\n");
				get_temper_val = 0;
			}
			printf("temp = %f",buf);
			write(cli_sockfd,&buf,sizeof(buf));
			read(cli_sockfd,&buf,sizeof(buf));
			printf("here is read and write\n");
			sleep(3);
		}
		close(cli_sockfd);
	}
	
	return 0;
}



//参数解析 域名解析
char *get_parameter(char *hostname)
{
	char *ipaddr;
	struct hostent *gethost;
	gethost = gethostbyname(hostname);
	char ip[30];
	if (NULL == gethost)
	{884
		perror("get host fail!");
		return NULL;
	}
	memset(ip,0,sizeof(ip));
	ipaddr = inet_ntop(gethost->h_addrtype,gethost->h_addr_list,ip,sizeof(ip));
	printf("dns ip is:%s\n",ipaddr);
	return ipaddr;
}



//服务器连接
int connect_server (char *ip,char *port)
{
	int sockfd = 0;
	struct sockaddr_in cli_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0)
	{
		perror("socket fail!");
		return -1;
	}
	printf("socket successfully!\n");
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family= AF_INET;
	cli_addr.sin_port = htons((uint16_t)port);
	printf("ready to connect!\n");
	if ((connect(sockfd,(struct sockaddr *)&cli_addr,sizeof(cli_addr))) < 0)
	{
		perror("connect fail!");
		connect_val = 0;
	}
	printf("connect successful!\n");
	connect_val = 1;
	return sockfd;
}



//读取温度值
int get_temper(float *temp)
{
	int fd;
	char *ptr;
	int rd;
	char buf[BUFFSIZE];
	char file_path[50] = "/home/bus/w1/devices/";
	DIR *dirp;
	struct dirent *direntp;
	int found = -1;
	char chip[20];
	dirp=opendir(file_path);
	if(dirp==NULL)
	{
		printf("open dir failure!:%s",strerror(errno));
		goto cleanup;
	}
	while((direntp=readdir(dirp))!=NULL)
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strcpy(chip,direntp->d_name);
			found=1;
			break;
		}
	}
	strcat(file_path,chip);
	strcat(file_path,"/w1_slave");

	fd=open(file_path,O_RDONLY,644);
	if(fd<0)
	{
		perror("open file failure!");
		return -1;
	}
	rd=read(fd,buf,sizeof(buf));
	close(fd);
	ptr=strstr(buf,"t=");
	ptr+=2;
	*temp=(atof(ptr)/1000.0);
cleanup:
	closedir(dirp);
	if(!found)
	{
		printf("can not find ds18d20\n");
	}
	return 0;
}
