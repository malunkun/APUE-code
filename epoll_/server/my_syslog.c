#include"server.h"
FILE *my_syslog(char *buf)
{
	FILE *fp;
	mode_t mode = umask(0);
	if(access("./log/",F_OK)==0)
		goto um;
	if(mkdir("./log/",0755)<0)
	{
		printf("创建日志文件夹失败:%s\n",strerror(errno));
		return NULL;
	}
um:
	umask(mode);
	fp = fopen(SYSLOG,"a+");
	if(NULL==fp)
		printf("打开日志文件失败:%s\n",strerror(errno));
	if(setvbuf(fp,buf,_IOLBF,128)!=0)
		return NULL;
	return fp;
}
void my_syslog_close(FILE *fp)
{
	fprintf(fp,"关闭日志文件！\n");
	fclose(fp);
}
