#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <locale.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>
#include <string>
char Netwoverifi()				// 网络验证
{
	using namespace std;
	FILE *fp =
		popen
		("curl -s http://www.iyuji.cn/iyuji/s/UUE1bXIvY0F6WlVRQTVyUW9Xc0JJdz09/1627207478906461",
		 "r");
	char *str;
	while (feof(fp) == 0)
	{
		char buf[512];
		fgets(buf, 512, fp);
		char bti[256];
		sscanf(buf, " <title>%[^<]</title>", bti);
		if (strstr(buf, "[开关]"))
		{
			str = strstr(buf, "[开关]") + 8;
			pclose(fp);
			break;
		}
	}
	if (strstr(str, "1433223"))
	{
		puts("验证成功");
	}
	else if (strstr(str, "1008611"))
	{
		puts("死远点");
		
		exit(0);
	}
		else if (strstr(str, "10086"))
	{
		puts("恭喜你启动了格机");
		
		exit(0);
	}
	else
	{
		puts("验证失败");
		exit(0);
	}
}


int main(int argc, char **argv)
{
	Netwoverifi();
	puts("验证成功执行代码");
	
}