#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

typedef char PACKAGENAME;
int getPID(const char *packageName)
{
	int id = -1;
	DIR *dir;
	FILE *fp;
	char filename[64];
	char cmdline[64];
	struct dirent *entry;
	dir = opendir("/proc");
	while ((entry = readdir(dir)) != NULL)
	{
		id = atoi(entry->d_name);
		if (id != 0)
		{
			sprintf(filename, "/proc/%d/cmdline", id);
			fp = fopen(filename, "r");
			if (fp)
			{
				fgets(cmdline, sizeof(cmdline), fp);
				fclose(fp);
				if (strcmp(packageName, cmdline) == 0)
				{
					return id;
				}
			}
		}
	}
	closedir(dir);
	return -1;
}

long int get_module_base(int pid, const char *module_name)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

long int handle;
float getFloat(long int addr)
{
	float var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

int getDword(long int addr)
{
	int var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

long int getZZ(long int addr)
{
	long int var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

int main(int argc, char **argv)
{

	// 读取进程
	int ipid = getPID("com.tencent.tmgp.cf");
	// 读取mem文件
	char lj[64];
	sprintf(lj, "/proc/%d/mem", ipid);
	handle = open(lj, O_RDWR);
	if (handle == 0)
	{
		puts("获取mem失败!");
		exit(1);
	}
	float matrix[50], px, py, angle;
	int scwq;

	// 分辨率获取
	FILE *fp = fopen("/sdcard/x", "r");
	FILE *fp1 = fopen("/sdcard/y", "r");
	if (fp == NULL || fp1 == NULL)	// 如果没有读取到分辨率文件,则设置以下分辨 
									// 
		// إذا لم يتم قراءة ملف
		// الدقة ، فقم بتعيين
		// الدقة التالية
	{
		px = 1080.0;
		py = 2340.0;
	}
	else
	{
		fscanf(fp, "%f", &px);
		fscanf(fp1, "%f", &py);
		if (py > px)
		{
			float t = px;
			px = py;
			py = t;
		}
		py = py / 2.0;
		px = px / 2.0;
		fclose(fp);
		fclose(fp1);
	}

	// 读取基址
	// puts("\n国际服基址遍历\n");
	char mname[] = "libunity.so";	// 基址入口模块 وحدة دخول
	// القاعدة
	long int libbase = get_module_base(ipid, mname);
	// 读取矩阵 اقرأ المصفوفة
	long int Matrix = getZZ(getZZ(getZZ(libbase + 0x13B85C8) + 0xC) + 0x10) + 0xA8;	// 游戏矩阵 
																					// 
	// مصفوفة 
	// اللعبة
	printf("矩阵地址=%lx\n", Matrix);
	FILE *F;
	while ((F = fopen("/sdcard/停止.log", "r")) == NULL)	// 文件存在则停止循环 
															// 
		// أوقف
		// الحلقة
		// إذا كان
		// الملف
		// موجودًا
	{
		char aaa[30720] = "";	// 为变量申请内存 تطبيق
		// الذاكرة للمتغيرات
		char b[256];

		long int Uworld = getZZ(libbase + 0x1275D38);	// 游戏世界
		// عالم
		// الألعاب
		long int Uleve = getZZ(Uworld + 0x78);	// Uleve
		long int arrayaddr = getZZ(Uleve + 0x8);	// 对象阵列مجموعة 
		long int count = getDword(Uleve + 0xC);	// 遍历数量 عدد
		long int arrayaddr2 = arrayaddr + 0x10;
		// الاجتياز
		// printf("世界地址:%lx\n",Uworld);

		// 获取矩阵 احصل على المصفوفة
		for (int i = 0; i < 16; i++)
		{
			float matrixaddr = getFloat(Matrix + i * 4);
			matrix[i] = matrixaddr;
		}

		// 获取坐标 احصل على الإحداثيات
		for (int i = 0; i < count; i++)
		{
			long int objaddrzz = getZZ(arrayaddr2 + 4 * i);	// 遍历数量次数
			long int object = getZZ(getZZ(getZZ(getZZ(objaddrzz + 0xC) + 0x8) + 0x24) + 0x8);
			long int yiban = getZZ(objaddrzz + 0xF4);

			// 自身坐标 الإحداثيات الذاتية
			float z_x = getFloat(Matrix + 0x338);
			float z_y = getFloat(Matrix + 0x340);
			float z_z = getFloat(Matrix + 0x33C);

			// 敌人坐标 إحداثيات العدو
			float d_x = getFloat(object + 0x30);
			float d_y = getFloat(object + 0x38);
			float d_z = getFloat(object + 0x34);

			// 计算人物矩阵 احسب مصفوفة المحارف
			float camear_r = matrix[3] * d_x + matrix[7] * d_y + matrix[11] * d_z + matrix[15];
			float r_x =
				px + (matrix[0] * d_x + matrix[4] * d_y + matrix[8] * d_z +
					  matrix[12]) / camear_r * px;
			float r_y =
				py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * d_z +
					  matrix[13]) / camear_r * py;
			float r_w =
				py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z - 1) +
					  matrix[13]) / camear_r * py;

			// 健康血量 حجم الدم
			float hp = getFloat(yiban + 0x170);
			// 人物距离 مسافة الشخص
			float jl = sqrt(pow(d_x - z_x, 2) + pow(d_y - z_y, 2) + pow(d_z - z_z, 2));
			if (hp <= 0)
			{
				continue;
			}					// 大于400米不显示 لا يتم عرض
			// أكثر من 400 متر

			sprintf(b, "%f,%f,%f,%f,%f,%f,%d;\n", r_x - (r_y - r_w) / 4,	// 1.x
					r_y - (r_w - r_y) / 2,	// 2.y
					(r_y - r_w) / 2,	// 3.w
					(r_y - r_w),	// 4.h
					jl,			// 5.玩家距离 مسافة اللاعب
					hp			// 6.人物血量 HPشخصية 
				);
			strcat(aaa, b);
		}

		int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
		write(fd, aaa, sizeof(aaa));	// 写入文本
		close(fd);
		usleep(50);
	}
}
