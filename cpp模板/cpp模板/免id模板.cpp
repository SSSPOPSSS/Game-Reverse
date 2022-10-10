#include <MemoryTools.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>
long int *posAddr = (long int *)malloc(8 * 100);
long int *jzAddr = (long int *)malloc(8 * 10);
float px = 2340.0 / 2.0;
float py = 1080.0 / 2.0;
//int jk = 1;
int zgs = 0;
float matrix[50];
void getm();
void getjz();

int main(int argc, char **argv)
{
	FILE *fd = NULL;
	fd = fopen("/sdcard/Gn.ini", "w+");
	fclose(fd);
	char *bm = "com.sofunny.Sausage";
	int a = 1;
	initMemoryTools(argv, "com.sofunny.Sausage");
	getm();
	std::thread t(getjz);
	for (;;)
	{
		char aaa[2048] = "";
		char b[164];
		
		//char *b = new char[164];
		//char *aaa = new char[1024];
        //char *b = (char *)malloc(8 * 100);
		
		
		int fd = -1;
		//int i;
		ADDRESSINFO bu;
		for (int i = 0; i < 16; i++)
		{
			GetAddressInfo(jzAddr[0] + 212 + i * 4, &bu);
			float fhjz = *(float *)bu.value;
			free(bu.value);
			matrix[i] = fhjz;
		}

		for (int i = 0; i < zgs; i++)
		{
			ADDRESSINFO date;
			
			// 血量
			GetAddressInfo(posAddr[i] - 3036, &date);
			float hp = *(float *)date.value;
			free(date.value);
			
			// 阵营(暂和谐)
			//GetAddressInfo(posAddr[i] - 4288, &date);
			//int xh = *(int *)date.value;
			//free(date.value);
			
			// 对象坐标
			GetAddressInfo(posAddr[i] - 1060, &date);
			float obj_x = *(float *)date.value;
			free(date.value);
			GetAddressInfo(posAddr[i] - 1056, &date);
			float obj_y = *(float *)date.value;
			free(date.value);
			GetAddressInfo(posAddr[i] - 1052, &date);
			float obj_z = *(float *)date.value;
			free(date.value);
		    
			// 去除队友
			GetAddressInfo(posAddr[i] - 1048, &date);
			float isr = *(float *)date.value;
			free(date.value);
			if (isr != 0.0){
			    continue;
			}
			
			// 自身坐标
			GetAddressInfo(jzAddr[0] + 196, &date);
			float z_x = *(float *)date.value;
			free(date.value);
			GetAddressInfo(jzAddr[0] + 200, &date);
			float z_y = *(float *)date.value;
			free(date.value);
			GetAddressInfo(jzAddr[0] + 204, &date);
			float z_z = *(float *)date.value;
			free(date.value);
			
			// 距离
			double pfg = sqrt(pow(obj_x - z_x, 2) + pow(obj_y - z_y, 2) + pow(obj_z - z_z, 2)) * 0.01;
			
			// 矩阵
			float camear_z = matrix[3] * obj_x + matrix[7] * obj_y + matrix[11] * obj_z + matrix[15];
			
			float r_x = px + (matrix[0] * obj_x + matrix[4] * obj_y + matrix[8] * obj_z + matrix[12]) / camear_z * px;
			
			float r_y = py - (matrix[1] * obj_x + matrix[5] * obj_y + matrix[9] * (obj_z - 5) + matrix[13]) / camear_z * py;
			
			float r_w = py - (matrix[1] * obj_x + matrix[5] * obj_y + matrix[9] * (obj_z + 205) + matrix[13]) / camear_z * py;
			
			sprintf(b, "%f,%f,%f,%f,%f,%f,%d;\n", r_x - (r_y - r_w) / 4 - 80, r_y, (r_y - r_w) / 2, r_y - r_w, pfg, hp, zgs);
			strcat(aaa, b);
		}
		fd = open("/sdcard/b.log", O_WRONLY);
		write(fd, aaa, sizeof(aaa));	// 写入文本
		close(fd);
		sleep(0.999);
		//delete b;
		//delete aaa;
		//ClearResults();
	}
}



void getm()
{
	void *jg;
	RESULTSINFO buf;
	char *bm = "com.sofunny.Sausage";
	SetSearchRange(A_ANONMYOUS);
	MemorySearch("0.33333334327",FLOAT );
	int gs = GetResultCount();	// 获取结果数量
	printf("共搜索%d个数据\n", gs);
	MemoryOffset("1.19175350666", 20, FLOAT);
	MemoryOffset("1.00010001659", 60, FLOAT);
	MemoryOffset("-50.00500106812", 76, FLOAT);
	MemoryOffset("-2147483648", 952, DWORD);
	gs = GetResultCount();
	printf("共偏移%d个数据\n", gs);
	GetResultsInfo(0, &buf);
	jzAddr[0] = buf.addr;
	free(buf.value);
	ClearResults();
}

void getjz()
{
	char *bm = "com.tencent.ig";
	SetSearchRange(A_ANONMYOUS);
    for(;;)
    {
	MemorySearch("479.5", FLOAT);
	MemoryOffset("3.0", 4, FLOAT);
	zgs = GetResultCount();
	RESULTSINFO zbdz;
	for (int i = 0; i < zgs; i++)
	{
		GetResultsInfo(i, &zbdz);
		posAddr[i] = zbdz.addr;
		free(zbdz.value);
	}
	ClearResults();
	sleep(5);
    }
}