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
	// 文字转码
#ifndef __UTF_H__
#define __UTF_H__
#define FALSE 0
#define TRUE 1
#define halfShift 10
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
	// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF
typedef unsigned char boolean;
typedef unsigned int CharType;
typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;

static const UTF32 halfMask = 0x3FFUL;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const UTF32 offsetsFromUTF8[6] =
	{ 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };
static const char trailingBytesForUTF8[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

typedef enum
{
	strictConversion = 0,
	lenientConversion
}
ConversionFlags;
typedef enum
{
	conversionOK,				// conversion successful
	sourceExhausted,			// partial character in source,but hit end
	targetExhausted,			// insuff. room in target for conversion
	sourceIllegal,				// source sequence is illegal/malformed
	conversionFailed
}
ConversionResult;
#endif

int Utf16_To_Utf8(const UTF16 * sourceStart, UTF8 * targetStart, size_t outLen,
				  ConversionFlags flags)
{
	int result = 0;
	const UTF16 *source = sourceStart;
	UTF8 *target = targetStart;
	UTF8 *targetEnd = targetStart + outLen;
	if ((NULL == source) || (NULL == targetStart))
	{
		// printf("ERR,Utf16_To_Utf8:source=%p,targetStart=%p\n",source,targetStart);
		return conversionFailed;
	}

	while (*source)
	{
		UTF32 ch;
		unsigned short bytesToWrite = 0;
		const UTF32 byteMask = 0xBF;
		const UTF32 byteMark = 0x80;
		const UTF16 *oldSource = source;	// In case we have to back up
		// because of target overflow.
		ch = *source++;
		// If we have a surrogate pair,convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			// If the 16 bits following the high surrogate are in the source
			// buffer...
			if (*source)
			{
				UTF32 ch2 = *source;
				// If it's a low surrogate,convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) +
						halfBase;
					++source;
				}
				else if (flags == strictConversion)
				{				// it's an unpaired high surrogate
					--source;	// return to the illegal value itself
					result = sourceIllegal;
					break;
				}
			}
			else
			{					// We don't have the 16 bits following the
				// high surrogate.
				--source;		// return to the high surrogate
				result = sourceExhausted;
				break;
			}
		}
		else if (flags == strictConversion)
		{
			// UTF-16 surrogate values are illegal in UTF-32
			if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
			{
				--source;		// return to the illegal value itself
				result = sourceIllegal;
				break;
			}
		}
		// Figure out how many bytes the result will require
		if (ch < (UTF32) 0x80)
		{
			bytesToWrite = 1;
		}
		else if (ch < (UTF32) 0x800)
		{
			bytesToWrite = 2;
		}
		else if (ch < (UTF32) 0x10000)
		{
			bytesToWrite = 3;
		}
		else if (ch < (UTF32) 0x110000)
		{
			bytesToWrite = 4;
		}
		else
		{
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}
		target += bytesToWrite;
		if (target > targetEnd)
		{
			source = oldSource;	// Back up source pointer!
			target -= bytesToWrite;
			result = targetExhausted;
			break;
		}
		switch (bytesToWrite)
		{						// note: everything falls through.
		case 4:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 3:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 2:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 1:
			*--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
		}
		target += bytesToWrite;
	}
	return result;
}
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

void WriteFloat(long int addr, float value)
{
	pwrite64(handle, &value, 4, addr);
}

/* 计算自瞄 */
#define PI 3.141592653589793238

double vax = 180 / PI, vay = 180 / PI;
int AimTrue;
double pfg;
double XGX, XGY;
float Myz, Myx, Myy, cx, cy, cz;
float JD, XGX1, XGY1, JD1;
int kh, myzt;
long int zmAddr;
long int zmAddrx, zmAddry;
float ZMx, ZMy;
float get2dDistance(float x, float y, float x1, float y1)
{
	float xx1 = x - x1;
	float yy2 = y - y1;
	// 取平方根
	return sqrt(xx1 * xx1 + yy2 * yy2);
}

int ab = 0;
bool KJ = false;
bool AimTren = true; //teue 开 false 关



int main(int argc, char **argv)
{
remove("/storage/emulated/0/GUA团队/仙人.txt");
FILE *hh=fopen("/sdcard/GUA团队/仙人.txt","r");//读取文件是否存在
if (hh == NULL)//如果读取不到文件,则执行↓
{
puts("未检测到验证文件");
exit(1);
//remove("/sdcard/");//删文件，自己复制写多个
//命令一：rebootsystem();//重启系统(手机)
//命令二：remove("填写文件或系统所在路径");//删除某个文件或系统
//命令三：uninstallapk("填写要卸载的软件包名");//静默卸载软件
}

	printf("\033[33;35m仙人交流群群:392289659\n\33[0m");
	printf("\033[33;37m本插件仅供学习参考\n\33[0m");
	printf("\033[33;38m请勿用于非法用途\n\33[0m");
	printf("\033[33;39m否则产生的后果与作者无关\n\n\33[0m");


	// 读取进程
	int ipid = getPID("com.tencent.tmgp.pubgmhd");

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
	FILE *fp = fopen("/sdcard/GUA团队/x", "r");
	FILE *fp1 = fopen("/sdcard/GUA团队/y", "r");
	if (fp == NULL || fp1 == NULL)	// 如果没有读取到分辨率文件,则设置以下分辨
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

	char mname[] = "libUE4.so";	// 基址入口模块
	long int libbase = get_module_base(ipid, mname);
	// 读取矩阵
	long int Matrix = getZZ(getZZ(libbase + 0xAD36EB0) + 0x98) + 0x320;	// 游戏矩阵
	FILE *F;
	while ((F = fopen("/sdcard/GUA团队/仙人.txt", "r")) == NULL)	// 文件存在则停止循环
	{
		char aaa[30720] = "";	// 为变量申请内存
		char b[256];
        long int Uworld = getZZ(libbase + 0xAD58338);	// 游戏世界
		long int Uleve = getZZ(Uworld + 0x30);	// Uleve
		long int arrayaddr = getZZ(Uleve + 0xa0);	// 对象阵列
		long int count = getDword(Uleve + 0xa8);	// 遍历数量
		long int oneself = getZZ(getZZ(getZZ(getZZ(Uworld + 0x38) + 0x78) + 0x30) + 0x3F0);	// 自身对象
        long int team = getDword(oneself + 0x8b0);	// 自身队伍编号

		// 获取矩阵
		for (int i = 0; i < 16; i++)
		{
			float matrixaddr = getFloat(Matrix + i * 4);
			matrix[i] = matrixaddr;
		}

		// 获取坐标
		ab = 0;
		for (int i = 0; i < count; i++)
		{
			long int objaddrzz = getZZ(arrayaddr + 4 * i);	// 遍历数量次数

			// 去除物资坐标
			float wzzb = getFloat(objaddrzz + 0x2524);
			if (wzzb != 479.5)
			{
				continue;
			}					// 不等于479.5则不输出坐标 

            int dw = getDword(objaddrzz + 0x8b0);
			if (team == dw)		// 不输出自己和队友坐标
			{
				continue;
			}
				
			long int object = getZZ(objaddrzz + 0x1D8);	// 对象地址指针

		    float z_x = getFloat(Matrix - 0x10);
			float z_y = getFloat(Matrix - 0xC);
			float z_z = getFloat(Matrix - 0x8);

			// 敌人坐标
			float d_x = getFloat(object + 0x1C0);
			float d_y = getFloat(object + 0x1C4);
			float d_z = getFloat(object + 0x1C8);

		
			// 计算人物矩阵
			float camear_r = matrix[3] * d_x + matrix[7] * d_y + matrix[11] * d_z + matrix[15];
			float r_x =
				px + (matrix[0] * d_x + matrix[4] * d_y + matrix[8] * d_z +
					  matrix[12]) / camear_r * px;
			float r_y =
				py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z - 5) +
					  matrix[13]) / camear_r * py;
			float r_w =
				py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z + 205) +
					  matrix[13]) / camear_r * py;

			zmAddr = getZZ(oneself + 0x304);
			zmAddrx = zmAddr + 0x304;
			zmAddry = zmAddr + 0x300;
			kh = getDword(zmAddr + 0xA74); // 开火 0为不开火 2为开火
			myzt = getDword(oneself + 0x2E8);	// 开镜 34为关镜 50为开镜

			float wx = getFloat(zmAddrx);
			float wy = getFloat(zmAddry);

			float Myx = getFloat(Matrix - 0x10);
			float Myy = getFloat(Matrix - 0xC);
			float Myz = getFloat(Matrix - 0x8);
			
			cx = d_x - Myx;
			cy = d_y - Myy;
			cz = d_z - Myz;

			pfg = sqrt((cx * cx) + (cy * cy));

			XGX = (float)atan2(cy, cx) * vax;
			XGY = (float)atan2(cz, pfg) * vay;

			if (XGX > -180 && XGX < 0)
			{
				XGX = XGX + 360;
			}
			JD = wx - XGX;
			JD = sqrt(JD * JD);
			if (ab == 0)
			{
				ab = 1;
				JD1 = JD + 1;
			}
			if (JD < JD1 && ab == 1)
			{
				JD1 = JD;
				XGX1 = XGX;
				XGY1 = XGY;
				ab = 1;
			}
			sprintf(b, "%d;\n", 			
					myzt
				);
			strcat(aaa, b);
		}
		if(AimTren){
		if (ab == 1 && myzt == 50)	// 开镜自瞄
		{
			WriteFloat(zmAddry, (XGY1 + 0.2525));
			WriteFloat(zmAddrx, XGX1);
		}
}
	
	}
}