/*************************************************************************
    > File Name: client.c
    > Author: malunkun
    > Mail: malunkun<209446860@qq.com> 
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
#include<time.h>

#define BUFFSIZE 1024
#define MALLOCSIZE 100
#define ID  "Ma001"

int connect_val = 0;
int get_temper_val = 0;


char *get_parameter(char *hostname);//参数解析
int connect_server (char *ip,int port);//连接服务器
int get_temper(float *temp);//获取温度

int main(int argc,char *argv[])
{
	int optret;
	char *ip = NULL;
	char ipArry[100];
	int  port = -1;
	char *hostname = NULL;
	int cli_sockfd = -1;
	float temp = 0.000;
	char buf[BUFFSIZE];
	time_t timep;
	time (&timep);//获取时间
	while((optret = getopt(argc,argv,"i:p:n:")) != -1)
	{
		switch(optret)
		{
			case 'i':
				printf("option = i,the ip will be set=%s\n",optarg);
				ip = optarg;
				break;
			case 'n':
				printf("option = h,the hostname will be set=%s\n",optarg);
				hostname = optarg;
				break;
			case 'p':
				printf("option = p,the port will be set=%s\n",optarg);
				port = atoi(optarg);
				break;
			default:
				printf("please put into as:[-i]ip [-p]port or [-h]hostname [-p]port\n");
				break;
		}
	}
	while(1)
	{
			if((ip != NULL)&&(hostname != NULL))
			{
				printf("please input only  ip or hostname!\n");
				return 0;
			}			
			if(hostname != NULL)
			{
				ip = get_parameter(hostname);
				strncpy(ipArry,ip,sizeof(ipArry));
				ip = (char *)ipArry;
				printf("get the ip by host is:%s\n",ip);
				hostname = NULL;
			}

			//printf("ip:%s\n",ip);
			
			while(!connect_val)
			{
				cli_sockfd = connect_server (ip,port);
				if (cli_sockfd < 0)
				{
					perror("connect fail!");
					printf("fd = %d\n",cli_sockfd);
					connect_val = 0;
				}
				if (cli_sockfd >0)
				{
					printf("connect successfully!\n");
					connect_val = 1;
				}
				sleep(3);
			}
		printf("the main will in file_io\n");


			while(!get_temper_val && connect_val)
			{
				if (get_temper(&temp) < 0)
				{
					perror("get temperature fail!\n");
					get_temper_val = 0;
				}
				printf("temp = %f\n",temp);
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%s/%.3f°C/%s",ID,temp,asctime(gmtime(&timep)));
				printf("buf_temp = %s\n",buf);
				write(cli_sockfd,&buf,sizeof(buf));
				read(cli_sockfd,&buf,sizeof(buf));
				sleep(5);
			}
			close(cli_sockfd);
			printf("cloes sockfd\n");
	}
		return 0;
}

//参数解析 域名解析
char *get_parameter(char *hostname)
{
	char *ipaddr = NULL;
	struct hostent *gethost;
	gethost = gethostbyname(hostname);
	char ip[100];
	if (NULL == gethost)
	{
		perror("get host fail!");
		return NULL;
	}
	memset(ip,0,sizeof(ip));
	ipaddr = (char *)inet_ntop(gethost->h_addrtype,gethost->h_addr_list[0],ip,sizeof(ip));
	//printf("dns ip is:%s\n",ipaddr);
	return ipaddr;
}

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
	//printf("connect successful!\n");
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
