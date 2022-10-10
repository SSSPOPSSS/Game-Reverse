#include <MemoryTools.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstring>

long int *posAddr = (long int *)malloc(8 * 100);
long int *jzAddr = (long int *)malloc(8 * 10);
int dis_zx[100] = {0};
float px;//宽度
float py;//高度

int zgs = 0;
float matrix[50];
void getm();
void getjz();
using namespace std;

int main(int argc,char **argv)
{
char *bm = "com.wooduan.ssjj.m4399";
int a = 1;
initMemoryTools(argv,bm,MODE_ROOT,"YnPArQSx");
FILE *F;
getm();
std::thread t(getjz);

//分辨率获取
FILE *fp=fopen("/sdcard/x","r");
//分辨率，格式:x,w
FILE *fp1=fopen("/sdcard/y","r");
//状态栏

if (fp == NULL || fp1 == NULL)//如果没有读取到分辨率文件,则设置以下分辨率
{
px = 2340.0;
py = 1080.0;

}
else
{
fscanf(fp,"%f",&px);
fscanf(fp1,"%f",&py);
if (py > px)
{
float t=px;
px=py;
py=t;
}

py = py / 2.0;
px = px / 2.0;
fclose(fp);
fclose(fp1);
}

while ((F=fopen("/sdcard/停止.log","r")) == NULL)
{
char aaa[2048] = "";//为变量请求内存
char b[256];
int fd = -1;
ADDRESSINFO bu;
for (int i = 0; i < 16; i++)
{
GetAddressInfo(jzAddr[0] + i * 4, &bu);//偏移到矩阵头
float fhjz = *(float *)bu.value;
free(bu.value);
matrix[i] = fhjz;
}

for (int i = 0; i < zgs; i++)
{
ADDRESSINFO date;

//血量
GetAddressInfo(posAddr[i] - 80, &date);
int hp = *(int *)date.value;
free(date.value);



// 对象坐标
GetAddressInfo(posAddr[i] + 28, &date);//x
float obj_x = *(float *)date.value;
free(date.value);

GetAddressInfo(posAddr[i] + 36, &date);//y
float obj_y = *(float *)date.value;
free(date.value);

GetAddressInfo(posAddr[i] + 32, &date);//z
float obj_z = *(float *)date.value;
free(date.value);

// 矩阵
float camear_z = matrix[3] * obj_x + matrix[7] * obj_z + matrix[11] * obj_y + matrix[15];

float r_x = px + (matrix[0] * obj_x + matrix[4] * obj_z + matrix[8] * obj_y + matrix[12]) / camear_z * px;

float r_y = py - (matrix[1] * obj_x + matrix[5] * (obj_z + 100) + matrix[9] * obj_y + matrix[13]) / camear_z * py;

float r_w = py - (matrix[1] * obj_x + matrix[5] * (obj_z - 80) + matrix[9] * obj_y + matrix[13]) / camear_z * py;

sprintf(b,"%f,%f,%f,%f,%d,%d\n",
r_x + 37,//x
r_w,//y
(r_y - r_w) /2,//h
(r_y - r_w),//w
hp//血量

);
strcat(aaa, b);
}
fd = open("/sdcard/b.log", O_WRONLY|O_CREAT);
write(fd, aaa, sizeof(aaa));//写入文本
close(fd);
usleep(100);
}
fclose(F);
return 0;
}



void getm()
{
void *jg;
RESULTSINFO buf;
char *bm = "com.wooduan.ssjj.m4399";//游戏包名
int gs;
SetSearchRange(C_ALLOC);
MemorySearch("3.5032461608120427E-44",TYPE_FLOAT);	
MemoryOffset("8.0",8,TYPE_FLOAT);
MemoryOffset("3.04081767e-43",4,TYPE_FLOAT);
gs = GetResultCount();
printf("共偏移%d个数据\n", gs);
PrintResults();
GetResultsInfo(0, &buf);
jzAddr[0] = buf.addr + 52;
free(buf.value);
ClearResults();
}
//矩阵搜索↑
void getjz()
{
char *bm = "com.wooduan.ssjj.m4399";
SetSearchRange(C_ALLOC);
for(;;)
{
MemorySearch("0.05000000074505806", TYPE_FLOAT);
MemoryOffset("49.0",4,TYPE_FLOAT);
MemoryOffset("1.0000000331813535E32",8,TYPE_FLOAT);
zgs = GetResultCount();
RESULTSINFO zbdz;
for (int i = 0; i < zgs; i++)
{
GetResultsInfo(i, &zbdz);
posAddr[i] = zbdz.addr;
free(zbdz.value);
}
ClearResults();
sleep(1);
}
}
//坐标搜索↑















