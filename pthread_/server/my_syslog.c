#include"server.h"
//日志系统
int my_syslog(int *sys_fd,int *syserr_fd)
{
	int fd;
	fd = open(SYSLOGFILE,O_CREAT|O_RDWR,0644);
	if(fd < 0)
	{
		perror("open syslog_file fail!\n");
		return -1;
	}
	*sys_fd = dup2(fd,STDOUT_FILENO);
	close(fd);
	lseek(*sys_fd,0,SEEK_END);
	fd = open(SYSERRFILE,O_CREAT|O_RDWR,0644);
	if(fd < 0)
	{
		perror("open syserrlog_file fail!");
		return -1;
	}
	*syserr_fd = dup2(fd,STDERR_FILENO);
	close(fd);
	lseek(*syserr_fd,0,SEEK_END);
	return 0;
}

