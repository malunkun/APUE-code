#include"client.h"
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
