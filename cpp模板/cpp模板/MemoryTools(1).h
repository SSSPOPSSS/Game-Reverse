/*
请使用c4droid,
并在设置里面开启
"以root身份运行"
*/
#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>

struct MAPS
{
long int addr;
long int taddr;
struct MAPS *next;
};

struct RESULT
{
long int addr;
struct RESULT *next;
};

struct FREEZE
{
long int addr;       //地址
char *value;         //值
int type;            //类型
struct FREEZE *next; //指向下一节点的指针
};

#define LEN sizeof(struct MAPS)
#define FRE sizeof(struct FREEZE)
typedef struct MAPS *PMAPS;    //存储maps的链表
typedef struct RESULT *PRES;   //存储结果的链表
typedef struct FREEZE *PFREEZE;//存储冻结的数据的链表

typedef int TYPE;
typedef int RANGE;
typedef int COUNT;
typedef int COLOR;
typedef long int OFFSET;
typedef long int ADDRESS;
typedef char PACKAGENAME;

enum type
{
DWORD,
FLOAT
};

enum Range
{
ALL,        //所有内存
B_BAD,      //B内存
C_ALLOC,    //Ca内存
C_BSS,      //Cb内存
C_DATA,     //Cd内存
C_HEAP,     //Ch内存
JAVA_HEAP,  //Jh内存
A_ANONMYOUS,//A内存
CODE_SYSTEM,//Xs内存
STACK,      //S内存
ASHMEM      //As内存
};

enum Color
{
COLOR_SILVERY,    //银色
COLOR_RED,        //红色
COLOR_GREEN,      //绿色
COLOR_YELLOW,     //黄色
COLOR_DARK_BLUE,  //蓝色
COLOR_PINK,       //粉色
COLOR_SKY_BLUE,   //天蓝
COLOR_WHITE       //白色
};

PMAPS Res=NULL;//全局buff(保存数据的地方)

PFREEZE Pfreeze=NULL;//用于存储冻结的数据
PFREEZE pEnd=NULL;
PFREEZE pNew=NULL;
int FreezeCount=0;//冻结数据个数
int Freeze=0;//开关
pthread_t pth;
char Fbm[64];//包名
long int delay=30000;//冻结延迟,默认30000us

int ResCount=0;//结果数量
int MemorySearchRange=0;//0为所有

int SetTextColor(int color);
int getRoot(char *argv[]);//获取root权限
int getPID(char bm[64]);//获取pid

int SetSearchRange(int type);//设置搜索范围
PMAPS readmaps(char *bm,int type);
PMAPS readmaps_all(char *bm);//读取maps文件
PMAPS readmaps_bad(char *bm);//读取maps文件
PMAPS readmaps_c_alloc(char *bm);//读取maps文件
PMAPS readmaps_c_bss(char *bm);//读取maps文件
PMAPS readmaps_c_data(char *bm);//读取maps文件
PMAPS readmaps_c_heap(char *bm);//读取maps文件
PMAPS readmaps_java_heap(char *bm);//读取maps文件
PMAPS readmaps_a_anonmyous(char *bm);//读取maps文件
PMAPS readmaps_code_system(char *bm);//读取maps文件
PMAPS readmaps_stack(char *bm);//读取maps文件
PMAPS readmaps_ashmem(char *bm);//读取maps文件

void BaseAddressSearch(char *bm,char *value,int *gs,int type,long int BaseAddr);//基址搜索
PMAPS BaseAddressSearch_DWORD(char *bm,int value,int *gs,long int BaseAddr,PMAPS pMap);//DWORD
PMAPS BaseAddressSearch_FLOAT(char *bm,float value,int *gs,long int BaseAddr,PMAPS pMap);//FLOAT

void RangeMemorySearch(char *bm,char *from_value,char *to_value,int *gs,int type);//范围搜索
PMAPS RangeMemorySearch_DWORD(char *bm,int from_value,int to_value,int *gs,PMAPS pMap);//DWORD
PMAPS RangeMemorySearch_FLOAT(char *bm,float from_value,float to_value,int *gs,PMAPS pMap);//FLOAT

void MemorySearch(char *bm,char *value,int *gs,int TYPE);//类型搜索,这里value需要传入一个地址
PMAPS MemorySearch_DWORD(char *bm,int value,int *gs,PMAPS pMap);  //内存搜索DWORD
PMAPS MemorySearch_FLOAT(char *bm,float value,int *gs,PMAPS pMap);  //内存搜索FLOAT

void MemoryOffset(char *bm,char *value,long int offset,int *gs,int type);//搜索偏移
PMAPS MemoryOffset_DWORD(char *bm,int value,long int offset,PMAPS pBuff,int *gs);//搜索偏移DWORD
PMAPS MemoryOffset_FLOAT(char *bm,float value,long int offset,PMAPS pBuff,int *gs);//搜索偏移FLOAT

void RangeMemoryOffset(char *bm,char *from_value,char *to_value,long int offset,int *gs,int type);//范围偏移
PMAPS RangeMemoryOffset_DWORD(char *bm,int from_value,int to_value,long int offset,PMAPS pBuff,int *gs);//搜索偏移DWORD
PMAPS RangeMemoryOffset_FLOAT(char *bm,float from_value,float to_value,long int offset,PMAPS pBuff,int *gs);//搜索偏移FLOAT

void MemoryWrite(char *bm,char *value,long int offset,int type);	//内存写入
int MemoryWrite_DWORD(char *bm,int value,PMAPS pBuff,long int offset);	//内存写入DWORD
int MemoryWrite_FLOAT(char *bm,float value,PMAPS pBuff,long int offset);	//内存写入FLOAT

void *SearchAddress(char *bm,long int addr);//搜索地址中的值,返回一个指针
int WriteAddress(char *bm,long int addr,void *value,int type);//修改地址中的值
void BypassGameSafe();//绕过游戏保护
//void RecBypassGameSafe(char *bm);//解除(停止使用)
void Print();//打印Res里面的内容
void ClearResults();//清除链表,释放空间
void ClearMaps(PMAPS pMap);//清空maps

int isapkinstalled(char *bm);//检测应用是否安装
int isapkrunning(char *bm);//检测应用是否运行
int killprocess(char *bm);//杀掉进程
char GetProcessState(char *bm);//获取进程状态
int killGG();//杀掉gg修改器
int killXs();//杀xs
int uninstallapk(char *bm);//静默删除软件
int installapk(char *lj);//静默卸载软件
int rebootsystem();//重启系统(手机)
int PutDate();//输出系统日期
int GetDate(char *date);//获取系统时间

PMAPS GetResults();//获取结果,返回头指针
int AddFreezeItem_All(char *bm,char *Value,int type,long int offset);//冻结所有结果
int AddFreezeItem(char *bm,long int addr,char *value,int type,long int offset);//增加冻结数据
int AddFreezeItem_DWORD(char *bm,long int addr,char *value);//DWORD
int AddFreezeItem_FLOAT(char *bm,long int addr,char *value);//FLOAT
int RemoveFreezeItem(long int addr);//清除固定冻结数据
int RemoveFreezeItem_All();//清空所有冻结数据
int StartFreeze(char *bm);//开始冻结
int StopFreeze();//停止冻结
int SetFreezeDelay(long int De);//设置冻结延迟
int PrintFreezeItems();//打印冻结表

int SetTextColor(COLOR color)
{
switch (color)
{
case COLOR_SILVERY:
printf("\033[30;1m");
break;
case COLOR_RED:
printf("\033[31;1m");
break;
case COLOR_GREEN:
printf("\033[32;1m");
break;
case COLOR_YELLOW:
printf("\033[33;1m");
break;
case COLOR_DARK_BLUE:
printf("\033[34;1m");
break;
case COLOR_PINK:
printf("\033[35;1m");
break;
case COLOR_SKY_BLUE:
printf("\033[36;1m");
break;
case COLOR_WHITE:
printf("\033[37;1m");
break;
default:
printf("\033[37;1m");
break;
}
return 0;
}

int getRoot(char *argv[])
{
char ml[64];
sprintf(ml,"su -c %s",*argv);
if (getuid() != 0)
{
system(ml);
exit(1);//退出没有root的进程
}
}

/*int getPID(PACKAGENAME bm[64])
{
FILE *fp;//文件指针
pid_t pid;//pid
char log[64];//命令
sprintf(log,"pidof %s > log.txt",bm);
system(log);//执行
if ((fp = fopen("log.txt", "r")) == NULL)
{
return -1;
}
fscanf(fp, "%d", &pid);//读取pid
remove("log.txt");
return pid;
}*/

int getPID(PACKAGENAME *PackageName)
{
DIR *dir=NULL;
struct dirent *ptr=NULL;
FILE *fp=NULL;
char filepath[256];			// 大小随意，能装下cmdline文件的路径即可
char filetext[128];			// 大小随意，能装下要识别的命令行文本即可
dir = opendir("/proc");		// 打开路径
if (NULL != dir)
{
while ((ptr = readdir(dir)) != NULL)	// 循环读取路径下的每一个文件/文件夹
{
// 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptr->d_type != DT_DIR)
continue;
sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);	// 生成要读取的文件的路径
fp = fopen(filepath, "r");	// 打开文件
if (NULL != fp)
{
fgets(filetext,sizeof(filetext),fp);	// 读取文件
if (strcmp(filetext,PackageName)==0)
{
//puts(filepath);
//printf("packagename:%s\n",filetext);
break;
}
fclose(fp);
}
}
}
if (readdir(dir) == NULL)
{
//puts("Get pid fail");
return 0;
}
closedir(dir);	// 关闭路径
return atoi(ptr->d_name);
}

int SetSearchRange(TYPE type)
{
switch (type)
{
case ALL:
MemorySearchRange=0;
break;
case B_BAD:
MemorySearchRange=1;
break;
case C_ALLOC:
MemorySearchRange=2;
break;
case C_BSS:
MemorySearchRange=3;
break;
case C_DATA:
MemorySearchRange=4;
break;
case C_HEAP:
MemorySearchRange=5;
break;
case JAVA_HEAP:
MemorySearchRange=6;
break;
case A_ANONMYOUS:
MemorySearchRange=7;
break;
case CODE_SYSTEM:
MemorySearchRange=8;
break;
case STACK:
MemorySearchRange=9;
break;
case ASHMEM:
MemorySearchRange=10;
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
return 0;
}

PMAPS readmaps(char* bm,TYPE type)
{
PMAPS pMap=NULL;
switch (type)
{
case ALL:
pMap=readmaps_all(bm);
break;
case B_BAD:
pMap=readmaps_bad(bm);
break;
case C_ALLOC:
pMap=readmaps_c_alloc(bm);
break;
case C_BSS:
pMap=readmaps_c_bss(bm);
break;
case C_DATA:
pMap=readmaps_c_data(bm);
break;
case C_HEAP:
pMap=readmaps_c_heap(bm);
break;
case JAVA_HEAP:
pMap=readmaps_java_heap(bm);
break;
case A_ANONMYOUS:
pMap=readmaps_a_anonmyous(bm);
break;
case CODE_SYSTEM:
pMap=readmaps_code_system(bm);
break;
case STACK:
pMap=readmaps_stack(bm);
break;
case ASHMEM:
pMap=readmaps_ashmem(bm);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pMap == NULL)
{
return 0;
}
return pMap;
}

PMAPS readmaps_all(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew;
PMAPS pEnd;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_bad(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"kgsl-3d0"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_c_alloc(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"[anon:libc_malloc]"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_c_bss(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"[anon:.bss]"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_c_data(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"/data/app/"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_c_heap(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"[heap]"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_java_heap(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"/dev/ashmem/"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_a_anonmyous(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && (strlen(buff) < 42))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_code_system(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"/system"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_stack(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"[stack]"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

PMAPS readmaps_ashmem(PACKAGENAME *bm)
{
PMAPS pHead=NULL;
PMAPS pNew=NULL;
PMAPS pEnd=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
FILE *fp;
int i = 0,flag=1;
char lj[64], buff[256];
int pid = getPID(bm);
sprintf(lj, "/proc/%d/maps", pid);
fp = fopen(lj, "r");
if (fp == NULL)
{
puts("分析失败");
return NULL;
}
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取一行
if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff,"/dev/ashmem/") && !strstr(buff,"dalvik"))
{
sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
//这里使用lx是为了能成功读取特别长的地址
flag=1;
}
else
{
flag=0;
}
if (flag==1)
{
i++;
if (i==1)
{
pNew->next=NULL;
pEnd=pNew;
pHead=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);//分配内存
}
}
free(pNew);//将多余的空间释放
fclose(fp);//关闭文件指针
return pHead;
}

void Print()
{
PMAPS temp = Res;
int i;
for (i=0;i<ResCount;i++)
{
printf("addr:0x%lX,taddr:0x%lX\n", temp->addr, temp->taddr);
temp = temp->next;//指向下一个节点
}
}

void ClearResults()//清空
{
PMAPS pHead=Res;
PMAPS pTemp=pHead;
int i;
for (i=0;i<ResCount;i++)
{
pTemp=pHead;
pHead=pHead->next;
free(pTemp);
}
}

void BaseAddressSearch(PACKAGENAME *bm,char *value,COUNT *gs,TYPE type,ADDRESS BaseAddr)
{
PMAPS pHead=NULL;
PMAPS pMap=NULL;
switch (MemorySearchRange)
{
case ALL:
pMap=readmaps(bm,ALL);
break;
case B_BAD:
pMap=readmaps(bm,B_BAD);
break;
case C_ALLOC:
pMap=readmaps(bm,C_ALLOC);
break;
case C_BSS:
pMap=readmaps(bm,C_BSS);
break;
case C_DATA:
pMap=readmaps(bm,C_DATA);
break;
case C_HEAP:
pMap=readmaps(bm,C_HEAP);
break;
case JAVA_HEAP:
pMap=readmaps(bm,JAVA_HEAP);
break;
case A_ANONMYOUS:
pMap=readmaps(bm,A_ANONMYOUS);
break;
case CODE_SYSTEM:
pMap=readmaps(bm,CODE_SYSTEM);
break;
case STACK:
pMap=readmaps(bm,STACK);
break;
case ASHMEM:
pMap=readmaps(bm,ASHMEM);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pMap == NULL)
{
puts("map error");
return (void)0;
}
switch (type)
{
case DWORD:
pHead=BaseAddressSearch_DWORD(bm,atoi(value),gs,BaseAddr,pMap);
break;
case FLOAT:
pHead=BaseAddressSearch_FLOAT(bm,atof(value),gs,BaseAddr,pMap);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pHead == NULL)
{
puts("search error");
return (void)0;
}
ResCount=*gs;
Res=pHead;//Res指针指向链表
}

PMAPS BaseAddressSearch_DWORD(PACKAGENAME *bm,int value,COUNT *gs,ADDRESS BaseAddr,PMAPS pMap)
{
*gs=0;
//printf("BaseAddress:%lX\n",BaseAddr);
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
PMAPS e,n;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff=n;
int iCount=0;
long int c,ADDR;
int handle;
char lj[64];
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
void *BUF[8];
PMAPS pTemp=pMap;
while (pTemp != NULL)
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j++)
{
ADDR=pTemp->addr+j*4096+BaseAddr;
pread64(handle,BUF,8,ADDR);
if (*(int*)&BUF[0] == value)
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=ADDR;
//printf("addr:%lx,val:%d,buff=%d\n",n->addr,value,buff[i]);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
}
pTemp=pTemp->next;
}
close(handle);
return pBuff;
}

PMAPS BaseAddressSearch_FLOAT(PACKAGENAME *bm,float value,COUNT *gs,ADDRESS BaseAddr,PMAPS pMap)
{
*gs=0;
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
PMAPS e,n;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff=n;
long int c,ADDR;
int handle;
int iCount=0;
char lj[64];
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
void *BUF[8];
PMAPS pTemp=pMap;
while (pTemp != NULL)
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j++)
{
ADDR=pTemp->addr+j*4096+BaseAddr;
pread64(handle,BUF,8,ADDR);
if (*(float*)&BUF[0] == value)
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=ADDR;
//printf("addr:%lx,val:%d,buff=%d\n",n->addr,value,buff[i]);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
}
pTemp=pTemp->next;
}
close(handle);
return pBuff;
}

void RangeMemorySearch(PACKAGENAME *bm,char *from_value,char *to_value,COUNT *gs,TYPE type)//范围搜索
{
PMAPS pHead=NULL;
PMAPS pMap=NULL;
switch (MemorySearchRange)
{
case ALL:
pMap=readmaps(bm,ALL);
break;
case B_BAD:
pMap=readmaps(bm,B_BAD);
break;
case C_ALLOC:
pMap=readmaps(bm,C_ALLOC);
break;
case C_BSS:
pMap=readmaps(bm,C_BSS);
break;
case C_DATA:
pMap=readmaps(bm,C_DATA);
break;
case C_HEAP:
pMap=readmaps(bm,C_HEAP);
break;
case JAVA_HEAP:
pMap=readmaps(bm,JAVA_HEAP);
break;
case A_ANONMYOUS:
pMap=readmaps(bm,A_ANONMYOUS);
break;
case CODE_SYSTEM:
pMap=readmaps(bm,CODE_SYSTEM);
break;
case STACK:
pMap=readmaps(bm,STACK);
break;
case ASHMEM:
pMap=readmaps(bm,ASHMEM);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pMap == NULL)
{
puts("map error");
return (void)0;
}
switch (type)
{
case DWORD:
if (atoi(from_value) > atoi(to_value))
pHead=RangeMemorySearch_DWORD(bm,atoi(to_value),atoi(from_value),gs,pMap);
else
pHead=RangeMemorySearch_DWORD(bm,atoi(from_value),atoi(to_value),gs,pMap);
break;
case FLOAT:
if (atof(from_value) > atof(to_value))
pHead=RangeMemorySearch_FLOAT(bm,atof(to_value),atof(from_value),gs,pMap);
else
pHead=RangeMemorySearch_FLOAT(bm,atof(from_value),atof(to_value),gs,pMap);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pHead == NULL)
{
puts("RangeSearch Error");
return (void)0;
}
ResCount=*gs;
Res=pHead;//Res指针指向链表
}

PMAPS RangeMemorySearch_DWORD(PACKAGENAME *bm,int from_value,int to_value,COUNT *gs,PMAPS pMap)//DWORD
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return NULL;
}
*gs=0;
PMAPS pTemp=NULL;
pTemp=pMap;
PMAPS n,e;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff;
pBuff=n;
int handle;//句柄
int iCount=0;//链表长度
int c;
char lj[64];//路径
int buff[1024]={0};//缓冲区
memset(buff,0,4);
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
while (pTemp != NULL)//读取maps里面的地址
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j++)
{
pread64(handle,buff,0x1000,pTemp->addr+j*4096);
for (int i=0;i<1024;i++)
{
if (buff[i] >= from_value && buff[i] <= to_value)//判断值是否在这两者之间
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=(pTemp->addr)+(j*4096)+(i*4);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
}
}
pTemp = pTemp->next;
}
free(n);
close(handle);
return pBuff;
}

PMAPS RangeMemorySearch_FLOAT(PACKAGENAME *bm,float from_value,float to_value,COUNT *gs,PMAPS pMap)//FLOAT
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return NULL;
}
*gs=0;
PMAPS pTemp=NULL;
pTemp=pMap;
PMAPS n,e;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff;
pBuff=n;
int handle;//句柄
int iCount=0;//链表长度
int c;
char lj[64];//路径
float buff[1024]={0};//缓冲区
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
while (pTemp->next != NULL)
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j+=1)
{
pread64(handle,buff,0x1000,pTemp->addr+(j*4096));
for (int i=0;i<1024;i+=1)
{
if (buff[i] >= from_value && buff[i] <= to_value)//判断。。。
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=(pTemp->addr)+(j*4096)+(i*4);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
//printf("buff[%d]=%f\n",l,buff[l]);
//usleep(1);
}
//memset(buff,0,4);
}
pTemp = pTemp->next;
}
free(n);
close(handle);
return pBuff;
}

void MemorySearch(PACKAGENAME *bm,char *value,int *gs,TYPE type)
{
PMAPS pHead=NULL;
PMAPS pMap=NULL;
switch (MemorySearchRange)
{
case ALL:
pMap=readmaps(bm,ALL);
break;
case B_BAD:
pMap=readmaps(bm,B_BAD);
break;
case C_ALLOC:
pMap=readmaps(bm,C_ALLOC);
break;
case C_BSS:
pMap=readmaps(bm,C_BSS);
break;
case C_DATA:
pMap=readmaps(bm,C_DATA);
break;
case C_HEAP:
pMap=readmaps(bm,C_HEAP);
break;
case JAVA_HEAP:
pMap=readmaps(bm,JAVA_HEAP);
break;
case A_ANONMYOUS:
pMap=readmaps(bm,A_ANONMYOUS);
break;
case CODE_SYSTEM:
pMap=readmaps(bm,CODE_SYSTEM);
break;
case STACK:
pMap=readmaps(bm,STACK);
break;
case ASHMEM:
pMap=readmaps(bm,ASHMEM);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pMap == NULL)
{
puts("map error");
return (void)0;
}
switch (type)
{
case DWORD:
pHead=MemorySearch_DWORD(bm,atoi(value),gs,pMap);
break;
case FLOAT:
pHead=MemorySearch_FLOAT(bm,atof(value),gs,pMap);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pHead == NULL)
{
puts("search error");
return (void)0;
}
ResCount=*gs;
Res=pHead;//Res指针指向链表
}

PMAPS MemorySearch_DWORD(PACKAGENAME *bm,int value,COUNT *gs,PMAPS pMap)
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return NULL;
}
*gs=0;
PMAPS pTemp=NULL;
pTemp=pMap;
PMAPS n,e;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff;
pBuff=n;
int handle;//句柄
int iCount=0;//链表长度
int c;
char lj[64];//路径
int buff[1024]={0};//缓冲区
memset(buff,0,4);
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
while (pTemp != NULL)//读取maps里面的地址
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j++)
{
pread64(handle,buff,0x1000,pTemp->addr+j*4096);
for (int i=0;i<1024;i++)
{
if (buff[i]==value)
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=(pTemp->addr)+(j*4096)+(i*4);
//printf("addr:%lx,val:%d,buff=%d\n",n->addr,value,buff[i]);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
}
}
pTemp = pTemp->next;
}
free(n);
close(handle);
return pBuff;
}

PMAPS MemorySearch_FLOAT(PACKAGENAME *bm,float value,COUNT *gs,PMAPS pMap)
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return NULL;
}
*gs=0;
PMAPS pTemp=NULL;
pTemp=pMap;
PMAPS n,e;
e=n=(PMAPS)malloc(LEN);
PMAPS pBuff;
pBuff=n;
int handle;//句柄
int iCount=0;//链表长度
int c;
char lj[64];//路径
float buff[1024]={0};//缓冲区
sprintf(lj, "/proc/%d/mem", pid);
handle = open(lj, O_RDWR);//打开mem文件
lseek(handle, 0, SEEK_SET);
while (pTemp->next != NULL)
{
c=(pTemp->taddr-pTemp->addr)/4096;
for (int j=0;j<c;j+=1)
{
pread64(handle,buff,0x1000,pTemp->addr+(j*4096));
for (int i=0;i<1024;i+=1)
{
if (buff[i]==value)
{
iCount++;
*gs+=1;
ResCount+=1;
n->addr=(pTemp->addr)+(j*4096)+(i*4);
if (iCount==1)
{
n->next=NULL;
e=n;
pBuff=n;
}
else
{
n->next=NULL;
e->next=n;
e=n;
}
n=(PMAPS)malloc(LEN);
}
//printf("buff[%d]=%f\n",l,buff[l]);
//usleep(1);
}
//memset(buff,0,4);
}
pTemp = pTemp->next;
}
free(n);
close(handle);
return pBuff;
}

void MemoryOffset(PACKAGENAME *bm,char *value,OFFSET offset,COUNT *gs,TYPE type)
{
PMAPS pHead=NULL;
switch (type)
{
case DWORD:
pHead=MemoryOffset_DWORD(bm,atoi(value),offset,Res,gs);
break;
case FLOAT:
pHead=MemoryOffset_FLOAT(bm,atof(value),offset,Res,gs);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pHead == NULL)
{
puts("offset error");
return (void)0;
}
ResCount=*gs;//全局个数
ClearResults();//清空存储的数据(释放空间)
Res=pHead;//指向新搜索到的空间
}

PMAPS MemoryOffset_DWORD(PACKAGENAME *bm,int value,OFFSET offset,PMAPS pBuff,COUNT *gs)//搜索偏移
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
*gs=0;//初始个数为0
PMAPS pEnd=NULL;
PMAPS pNew=NULL;
PMAPS pTemp=pBuff;
PMAPS BUFF=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
BUFF=pNew;
int iCount=0,handle;//个数与句柄
char lj[64];//路径
long int all;//总和
int *buf=(int *)malloc(sizeof(int));//缓冲区
int jg;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
while (pTemp != NULL)
{
all=pTemp->addr+offset;
pread64(handle,buf,4,all);
jg=*buf;
if (jg == value)
{
iCount++;
*gs+=1;
pNew->addr=pTemp->addr;
if (iCount == 1)
{
pNew->next=NULL;
pEnd=pNew;
BUFF=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);
if (ResCount==1)
{
free(pNew);
close(handle);
return BUFF;
}
}
/*else
{
printf("jg:%d,value:%d\n",jg,value);
}*/
pTemp=pTemp->next;//指向下一个节点读取数据
}
free(pNew);
close(handle);
return BUFF;
}

PMAPS MemoryOffset_FLOAT(PACKAGENAME *bm,float value,OFFSET offset,PMAPS pBuff,COUNT *gs)//搜索偏移
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
*gs=0;//初始个数为0
PMAPS pEnd=NULL;
PMAPS pNew=NULL;
PMAPS pTemp=pBuff;
PMAPS BUFF=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
BUFF=pNew;
int iCount=0,handle;//个数与句柄
char lj[64];//路径
long int all;//总和
float *buf=(float *)malloc(sizeof(float));//缓冲区
//int buf[16];  //出现异常使用
float jg;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
while (pTemp != NULL)
{
all=pTemp->addr+offset;//偏移后的地址
pread64(handle,buf,4,all);
jg=*buf;
if (jg == value)
{
iCount++;
*gs+=1;
//printf("偏移成功,addr:%lx\n",all);
pNew->addr=pTemp->addr;
if (iCount == 1)
{
pNew->next=NULL;
pEnd=pNew;
BUFF=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);
if (ResCount==1)
{
free(pNew);
close(handle);
return BUFF;
}
}
/*else
{
printf("jg:%e,value:%e\n",jg,value);
}*/
pTemp=pTemp->next;//指向下一个节点读取数据
}
free(pNew);
close(handle);
return BUFF;
}

void RangeMemoryOffset(PACKAGENAME *bm,char *from_value,char *to_value,OFFSET offset,COUNT *gs,TYPE type)//范围偏移
{
PMAPS pHead=NULL;
switch (type)
{
case DWORD:
if (atoi(from_value) > atoi(to_value))
pHead=RangeMemoryOffset_DWORD(bm,atoi(to_value),atoi(from_value),offset,Res,gs);
else
pHead=RangeMemoryOffset_DWORD(bm,atoi(from_value),atoi(to_value),offset,Res,gs);
break;
case FLOAT:
if (atof(from_value) > atof(to_value))
pHead=RangeMemoryOffset_FLOAT(bm,atof(to_value),atof(from_value),offset,Res,gs);
else
pHead=RangeMemoryOffset_FLOAT(bm,atof(from_value),atof(to_value),offset,Res,gs);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
if (pHead == NULL)
{
puts("RangeOffset error");
return (void)0;
}
ResCount=*gs;//全局个数
ClearResults();//清空存储的数据(释放空间)
Res=pHead;//指向新搜索到的空间
}

PMAPS RangeMemoryOffset_DWORD(PACKAGENAME *bm,int from_value,int to_value,OFFSET offset,PMAPS pBuff,COUNT *gs)//搜索偏移DWORD
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
*gs=0;//初始个数为0
PMAPS pEnd=NULL;
PMAPS pNew=NULL;
PMAPS pTemp=pBuff;
PMAPS BUFF=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
BUFF=pNew;
int iCount=0,handle;//个数与句柄
char lj[64];//路径
long int all;//总和
int *buf=(int *)malloc(sizeof(int));//缓冲区
int jg;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
while (pTemp != NULL)
{
all=pTemp->addr+offset;
pread64(handle,buf,4,all);
jg=*buf;
if (jg >= from_value && jg <= to_value)
{
iCount++;
*gs+=1;
pNew->addr=pTemp->addr;
if (iCount == 1)
{
pNew->next=NULL;
pEnd=pNew;
BUFF=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);
if (ResCount==1)
{
free(pNew);
close(handle);
return BUFF;
}
}
/*else
{
printf("jg:%d,value:%d\n",jg,value);
}*/
pTemp=pTemp->next;//指向下一个节点读取数据
}
free(pNew);
close(handle);
return BUFF;
}

PMAPS RangeMemoryOffset_FLOAT(PACKAGENAME *bm,float from_value,float to_value,OFFSET offset,PMAPS pBuff,COUNT *gs)//搜索偏移FLOAT
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
*gs=0;//初始个数为0
PMAPS pEnd=NULL;
PMAPS pNew=NULL;
PMAPS pTemp=pBuff;
PMAPS BUFF=NULL;
pEnd=pNew=(PMAPS)malloc(LEN);
BUFF=pNew;
int iCount=0,handle;//个数与句柄
char lj[64];//路径
long int all;//总和
float *buf=(float *)malloc(sizeof(float));//缓冲区
//int buf[16];  //出现异常使用
float jg;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
while (pTemp != NULL)
{
all=pTemp->addr+offset;//偏移后的地址
pread64(handle,buf,4,all);
jg=*buf;
if (jg >= from_value && jg <= to_value)
{
iCount++;
*gs+=1;
//printf("偏移成功,addr:%lx\n",all);
pNew->addr=pTemp->addr;
if (iCount == 1)
{
pNew->next=NULL;
pEnd=pNew;
BUFF=pNew;
}
else
{
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
}
pNew=(PMAPS)malloc(LEN);
if (ResCount==1)
{
free(pNew);
close(handle);
return BUFF;
}
}
/*else
{
printf("jg:%e,value:%e\n",jg,value);
}*/
pTemp=pTemp->next;//指向下一个节点读取数据
}
free(pNew);
close(handle);
return BUFF;
}

void MemoryWrite(PACKAGENAME *bm,char *value,OFFSET offset,TYPE type)
{
switch (type)
{
case DWORD:
MemoryWrite_DWORD(bm,atoi(value),Res,offset);
break;
case FLOAT:
MemoryWrite_FLOAT(bm,atof(value),Res,offset);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
//ClearResults();//清空list
}

int MemoryWrite_DWORD(PACKAGENAME *bm,int value,PMAPS pBuff,OFFSET offset)
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
PMAPS pTemp=NULL;
char lj[64];
int handle;
pTemp=pBuff;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
int i;
for (i=0;i<ResCount;i++)
{
pwrite64(handle,&value,4,pTemp->addr+offset);
if (pTemp->next != NULL)
pTemp=pTemp->next;
}
close(handle);
return 0;
}

int MemoryWrite_FLOAT(PACKAGENAME *bm,float value,PMAPS pBuff,OFFSET offset)
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
PMAPS pTemp=NULL;
char lj[64];
int handle;
pTemp=pBuff;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
int i;
for (i=0;i<ResCount;i++)
{
pwrite64(handle,&value,4,pTemp->addr+offset);
if (pTemp->next != NULL)
pTemp=pTemp->next;
}
close(handle);
return 0;
}

void *SearchAddress(PACKAGENAME *bm,ADDRESS addr)//返回一个void指针,可以自行转换类型
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
char lj[64];
int handle;
void *buf=malloc(8);
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
pread64(handle,buf,8,addr);
close(handle);
return buf;
}

int WriteAddress(PACKAGENAME *bm,ADDRESS addr,void *value,TYPE type)
{
pid_t pid=getPID(bm);
if (pid == 0)
{
puts("can not get pid");
return 0;
}
char lj[64];
int handle;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
lseek(handle,0,SEEK_SET);
switch (type)
{
case DWORD:
pwrite64(handle,(int*)value,4,addr);
break;
case FLOAT:
pwrite64(handle,(float*)value,4,addr);
break;
default:
printf("\033[32;1mYou Select A NULL Type!\n");
break;
}
close(handle);
return 0;
}

int isapkinstalled(PACKAGENAME *bm)
{
char LJ[128];
sprintf(LJ,"/data/data/%s/",bm);
DIR *dir;
dir=opendir(LJ);
if (dir == NULL)
{
return 0;
}
else
{
return 1;
}
}

int isapkrunning(PACKAGENAME *bm)
{
DIR *dir=NULL;
struct dirent *ptr=NULL;
FILE *fp=NULL;
char filepath[50];			// 大小随意，能装下cmdline文件的路径即可
char filetext[128];			// 大小随意，能装下要识别的命令行文本即可
dir = opendir("/proc/");		// 打开路径
if (dir != NULL)
{
while ((ptr = readdir(dir)) != NULL)	// 循环读取路径下的每一个文件/文件夹
{
// 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptr->d_type != DT_DIR)
continue;
sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);	// 生成要读取的文件的路径
fp = fopen(filepath, "r");	// 打开文件
if (NULL != fp)
{
fgets(filetext,sizeof(filetext),fp);	// 读取文件
if (strcmp(filetext,bm)==0)
{
closedir(dir);
return 1;
}
fclose(fp);
}
}
}
closedir(dir);	// 关闭路径
return 0;
}

int uninstallapk(PACKAGENAME *bm)
{
char ml[128];
sprintf(ml,"pm uninstall %s",bm);
system(ml);
clrscr();
return 0;
}

int installapk(char *lj)
{
char ml[128];
sprintf(ml,"pm install %s",lj);
system(ml);
clrscr();
return 0;
}

int killprocess(PACKAGENAME *bm)
{
int pid=getPID(bm);
if (pid == 0)
{
return -1;
}
char ml[32];
sprintf(ml,"kill %d",pid);
system(ml);//杀掉进程
return 0;
}

char GetProcessState(PACKAGENAME *bm)
{
/*
D 无法中断的休眠状态（通常 IO 的进程）；
R 正在运行，在可中断队列中；
S 处于休眠状态，静止状态；
T 停止或被追踪，暂停执行；
W 进入内存交换（从内核2.6开始无效）；
X 死掉的进程；
Z 僵尸进程不存在但暂时无法消除；
W: 没有足够的记忆体分页可分配WCHAN 正在等待的进程资源；
<: 高优先级进程
N: 低优先序进程
L: 有记忆体分页分配并锁在记忆体内 (即时系统或捱A I/O)，即,有些页被锁进内存
s 进程的领导者（在它之下有子进程）；
l 多进程的（使用 CLONE_THREAD, 类似 NPTL pthreads）；
+ 位于后台的进程组；
*/
int pid=getPID(bm);//获取pid
if (pid == 0)
{
return 0;//无进程
}
FILE *fp;
char lj[64];
char buff[64];
char zt;
char zt1[16];
sprintf(lj,"/proc/%d/status",pid);
fp = fopen(lj,"r");//打开status文件
if (fp == NULL)
{
return 0;//无进程
}
//puts("loop");
while (!feof(fp))
{
fgets(buff,sizeof(buff),fp);//读取
if (strstr(buff,"State"))//筛选
{
sscanf(buff,"State: %c",&zt);//选取
//printf("state:%c\n",zt);
//sleep(1);
//puts("emmmm");
break;//跳出循环
}
}
//putchar(zt);
//puts(zt2);
fclose(fp);
//puts("loopopp");
return zt;//返回状态
}

int rebootsystem()
{
return system("su -c 'reboot'");
}

int PutDate()
{
return system("date +%F-%T");
}

int GetDate(char *date)
{
FILE *fp;//文件指针
system("date +%F-%T > log.txt");//执行
if ((fp = fopen("log.txt", "r")) == NULL)
{
return 0;
}
fscanf(fp,"%s",date);//读取
remove("log.txt");//删除
return 1;
}

void BypassGameSafe()
{
system("echo 0 > /proc/sys/fs/inotify/max_user_watches");
//char ml[80];
//sprintf(ml,"chmod 444 /data/data/%s/files",bm);
//system(ml);
}

int killGG()//杀掉GG修改器
{
//在/data/data/[GG修改器包名]/files/里面有一个文件夹名字是GG-****
//如果有这个文件夹，就获取上面所说的包名，杀掉GG修改器
DIR *dir=NULL;
DIR *dirGG=NULL;
struct dirent *ptr=NULL;
struct dirent *ptrGG=NULL;
char filepath[256];			// 大小随意，能装下cmdline文件的路径即可
char filetext[128];			// 大小随意，能装下要识别的命令行文本即可
dir = opendir("/data/data");		// 打开路径
//puts("正在杀GG");
int flag=1;
if (dir != NULL)
{
while (flag && (ptr = readdir(dir)) != NULL)	// 循环读取路径下的每一个文件/文件夹
{
// 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptr->d_type != DT_DIR)
continue;
sprintf(filepath, "/data/data/%s/files", ptr->d_name);	// 生成要读取的文件的路径
dirGG = opendir(filepath);	// 打开文件
if (dirGG != NULL)
{
while ((ptrGG = readdir(dirGG)) != NULL)
{
if ((strcmp(ptrGG->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptrGG->d_type != DT_DIR)
continue;
if (strstr(ptrGG->d_name,"GG"))//判断文件夹名字
{
int pid;//pid
pid = getPID(ptr->d_name);//获取GG包名
//ptr->d_name存储文件名字(也就是软件包名)
if (pid == 0)//如果pid是0，代表GG没有运行
continue;
else//如果成功获取pid
killprocess(ptr->d_name);
}
}
}
/*else
{
puts(filepath);//调试
}*/
}
}
closedir(dir);	// 关闭
closedir(dirGG);
return 0;
}

int killXs()//杀掉Xs
{
DIR *dir=NULL;
struct dirent *ptr=NULL;
char filepath[256];			// 大小随意，能装下cmdline文件的路径即可
char filetext[128];			// 大小随意，能装下要识别的命令行文本即可
dir = opendir("/data/data");		// 打开路径
FILE *fp=NULL;
if (NULL != dir)
{
while ((ptr = readdir(dir)) != NULL)	// 循环读取路径下的每一个文件/文件夹
{
// 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptr->d_type != DT_DIR)
continue;
///data/data/%s/lib/libxscript.so
sprintf(filepath, "/data/data/%s/lib/libxscript.so", ptr->d_name);	// 生成要读取的文件的路径
fp=fopen(filepath,"r");
if (fp == NULL)
continue;
else//如果读取成功(xs正在运行)
{
killprocess(ptr->d_name);
//杀进程
}
//killprocess(ptr->d_name);
}
}
closedir(dir);	// 关闭
return 0;
}

/*void RecBypassGameSafe(char *bm)
{
char ml[80];
sprintf(ml,"chmod 771 /data/data/%s/files",bm);
system(ml);
}*/

void *FreezeThread(void* a)
{
int handle;
int pid;
pid=getPID(Fbm);
if (pid == 0)
{
puts("Error -1");
return (void*)0;
}
char lj[64];
int buf_i;
float buf_f;
sprintf(lj,"/proc/%d/mem",pid);
handle=open(lj,O_RDWR);
if (handle == -1)
{
puts("Error -2");
return (void*)0;
}
lseek(handle,0,SEEK_SET);
PFREEZE pTemp=Pfreeze;
while (Freeze == 1)
{
for (int i=0;i<FreezeCount;i++)
{
switch (pTemp->type)
{
case DWORD:
buf_i=atoi(pTemp->value);
pwrite64(handle,&buf_i,4,pTemp->addr);
break;
case FLOAT:
buf_f=atof(pTemp->value);
pwrite64(handle,&buf_f,4,pTemp->addr);
break;
default:
break;
}
pTemp=pTemp->next;
usleep(delay);
}
pTemp=Pfreeze;//重新指向头指针
}
return NULL;
}

PMAPS GetResults()//获取搜索出的结果
{
if (Res == NULL)
{
return NULL;
}
else
{
return Res;//返回头指针
}
}

int AddFreezeItem_All(PACKAGENAME *bm,char *Value,TYPE type,OFFSET offset)
{
if (ResCount == 0)
{
return -1;
}
PMAPS pTemp=Res;
for (int i=0;i<ResCount;i++)
{
switch (type)
{
case DWORD:
AddFreezeItem(bm,pTemp->addr,Value,DWORD,offset);
break;
case FLOAT:
AddFreezeItem(bm,pTemp->addr,Value,FLOAT,offset);
break;
default:
SetTextColor(COLOR_SKY_BLUE);
puts("You Choose a NULL type");
break;
}
pTemp=pTemp->next;
}
return 0;
}

int AddFreezeItem(PACKAGENAME *bm,ADDRESS addr,char *value,TYPE type,OFFSET offset)
{
switch (type)
{
case DWORD:
AddFreezeItem_DWORD(bm,addr+offset,value);
break;
case FLOAT:
AddFreezeItem_FLOAT(bm,addr+offset,value);
break;
default:
SetTextColor(COLOR_SKY_BLUE);
puts("You Choose a NULL type");
break;
}
return 0;
}

int AddFreezeItem_DWORD(PACKAGENAME *bm,ADDRESS addr,char *value)
{
if (FreezeCount == 0)//如果没有数据
{
Pfreeze=pEnd=pNew=(PFREEZE)malloc(FRE);//分配新空间
pNew->next=NULL;
pEnd=pNew;
Pfreeze=pNew;
pNew->addr=addr;//储存地址
pNew->type=DWORD;
pNew->value=value;
FreezeCount+=1;
}
else
{
pNew=(PFREEZE)malloc(FRE);//分配空间
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
pNew->addr=addr;
pNew->type=DWORD;
pNew->value=value;
FreezeCount+=1;
}
return 0;
}

int AddFreezeItem_FLOAT(PACKAGENAME *bm,ADDRESS addr,char *value)
{
if (FreezeCount == 0)//如果没有数据
{
Pfreeze=pEnd=pNew=(PFREEZE)malloc(FRE);//分配新空间
pNew->next=NULL;
pEnd=pNew;
Pfreeze=pNew;
pNew->addr=addr;//储存地址
pNew->type=FLOAT;
pNew->value=value;
FreezeCount+=1;
}
else
{
pNew=(PFREEZE)malloc(FRE);//分配空间
pNew->next=NULL;
pEnd->next=pNew;
pEnd=pNew;
pNew->addr=addr;
pNew->type=FLOAT;
pNew->value=value;
FreezeCount+=1;
}
return 0;
}

int RemoveFreezeItem(ADDRESS addr)
{
PFREEZE pTemp=Pfreeze;
PFREEZE p1=NULL;
PFREEZE p2=NULL;
for (int i=0;i<FreezeCount;i++)
{
p1=pTemp;
p2=pTemp->next;
if (pTemp->addr == addr)
{
p1->next=p2;
free(pTemp);
FreezeCount-=1;
//printf("冻结个数:%d\n",FreezeCount);
//break;//防止地址重复冻结，所以不加，当然也可以加上
}
pTemp=p2;
}
return 0;
}

int RemoveFreezeItem_All()
{
PFREEZE pHead=Pfreeze;
PFREEZE pTemp=pHead;
int i;
for (i=0;i<FreezeCount;i++)
{
pTemp=pHead;
pHead=pHead->next;
free(pTemp);
FreezeCount-=1;
}
free(Pfreeze);
FreezeCount-=1;
return 0;
}

int StartFreeze(PACKAGENAME *bm)
{
if (Freeze == 1)//已经开启冻结
{
return -1;
}
int a;
strcpy(Fbm,bm);//******
Freeze=1;
pthread_create(&pth,NULL,FreezeThread,NULL);
//创建线程(开始冻结线程)
return 0;
}

int StopFreeze()
{
Freeze=0;
return 0;
}

int SetFreezeDelay(long int De)
{
delay=De;
return 0;
}

int PrintFreezeItems()
{
PFREEZE pTemp=Pfreeze;
for (int i=0;i<FreezeCount;i++)
{
printf("FreezeAddr:%lx,type:%s,value:%s\n",pTemp->addr,pTemp->type == DWORD ? "DWORD":"FLOAT",pTemp->value);
pTemp=pTemp->next;
}
return 0;
}