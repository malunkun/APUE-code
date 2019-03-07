#include"client.h"
int go_stop = 0;

void hander(int num)
{
	go_stop = 1;
}
int main(int argc,char *argv[])
{
	int go_stop = 0;
	int connect_val = 0;
	int get_temper_val = 0;
	int optret;
	int sys_fd = -1;
	int syserr_fd = -1;
	char *ip = NULL;
	char ipArry[100];
	int  port = -1;
	char *hostname = NULL;
	int cli_sockfd = -1;
	float temp = 0.000;
	char buf[BUFFSIZE];
	time_t timep;
	mode_t mode = umask(0);
	mkdir("./debug/",0777);
	signal(SIGQUIT,hander);
	optret = my_syslog(&sys_fd,&syserr_fd);//开启日志
	if(optret < 0)
	{
		printf("syslog open fail!\n");
		return -1;
	}
	daemon(0,1);  //守护进程
	time (&timep);//获取时间
	while((optret = getopt(argc,argv,"i:p:n:")) != -1)
	{
		switch(optret)
		{
			case 'i':
				printf("option = i,the ip or hostname will be set=%s\n",optarg);
				hostname = optarg;
				optret = get_parameter(hostname,ipArry);
				if(optret < 0)
				{
					ip = hostname;
				}
				ip = (char *)ipArry;
				break;
			case 'p':
				printf("option = p,the port will be set=%s\n",optarg);
				port = atoi(optarg);
				break;
			default:
				printf("please put into as:[-i]ip [-p]port\n");
				break;
				fflush(stdout);
				fflush(stderr);
		}
	}

	while(!go_stop)
	{
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
				fflush(stdout);
				fflush(stderr);
			}
			while(!get_temper_val && connect_val)
			{
				if (get_temper(&temp) < 0)
				{
					perror("get temperature fail!\n");
					get_temper_val = 0;
				}
				printf("temp = %f\n",temp);
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%s|%.3f°C|%s",ID,temp,asctime(gmtime(&timep)));
				printf("buf_temp = %s\n",buf);
				if(write(cli_sockfd,&buf,sizeof(buf)) <0)
				{
					perror("write fail!");
					connect_val = 0;
					break;
				}
				if(read(cli_sockfd,&buf,sizeof(buf)) <= 0)
				{
					perror("read fail!");
					connect_val = 0;
					break;
				}
				sleep(5);
				fflush(stdout);
				fflush(stderr);
			}
			close(cli_sockfd);
			printf("cloes sockfd\n");
			fflush(stdout);
			fflush(stderr);
			close(sys_fd);
			close(syserr_fd);
			umask(mode);
	}
		return 0;
}
