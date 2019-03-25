#include"client.h"
void my_time(char *buf)
{
	time_t t;
	t = time(0);
	strftime(buf,200,"%Y年%m月%d日 %H:%M:%S", localtime(&t));
}
