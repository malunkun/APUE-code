#include"server.h"
void pthread_start(int fd)//////线程开启
{
	pthread_t tid;
	pthread_attr_t thread_attr;
	if (pthread_attr_init(&thread_attr))
	{
		perror("pthread_attr_init() fail!");
	}
	if (pthread_attr_setstacksize(&thread_attr,120*1024))
	{
		perror("pthread_attr_setstacksize() fail!");
	}
	if (pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED))
	{
		perror("pthread_attr_setdetachstate() fail!");
	}
	pthread_create(&tid,&thread_attr,pthread_func,(void *)fd);

cleanup:
	pthread_attr_destroy(&thread_attr);	
	printf("destroy attr successful!\n");
}

void *pthread_func(void *args)
{
	int plisten_fd = (int)args;
	char buf[BUFFSIZE];
	int sqlite_ret;
	int rd;
	while(1)
	{
		memset(buf,0,sizeof(buf));
		rd = read(plisten_fd,buf,sizeof(buf));
		if (rd <= 0)
		{
			perror("read fail!");
			break;
		}
		if(write(plisten_fd,buf,sizeof(buf)) < 0)
		{
			perror("write fail!");
			break;
		}
		printf("%s\n",buf);
		sqlite_ret = sqlite_func((char *)&buf);
	} 
		close(plisten_fd);
        fflush(stdout);
        fflush(stderr);
		return NULL;
}
