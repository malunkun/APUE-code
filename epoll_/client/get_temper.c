#include"client.h"
//读取温度值
int get_temper(float *temp,FILE *fp)
{
	int fd;
	char *ptr;
	int rd;
	char buf[1024];
	char file_path[50] = TPATH;
	DIR *dirp;
	struct dirent *direntp;
	int found = -1;
	char chip[20];
	dirp=opendir(file_path);
	if(dirp==NULL)
	{
		fprintf(fp,"打开文件夹失败:%s",strerror(errno));
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
		fprintf(fp,"打开文件失败:%s\n",strerror(errno));
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
		fprintf(fp,"找不到设备\n");
	}
	return 0;
}
