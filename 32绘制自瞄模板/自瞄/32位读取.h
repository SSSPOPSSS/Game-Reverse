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
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>

#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

//坐标计算宏
#define MaxPlayerCount 30
#define MaxVehicleCount 20
#define MaxItemsCount 20
#define MaxGrenadeCount 20
#define MaxAirBoxCount 20

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

//公共变量区域
int ipid,handle,scwq;;
float matrix[50], px, py, angle;

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


/*下面是函数区域*/

//Open_Memory Function
int Open_Memory()
{
	char lj[64];
	if(ipid == -1)
	{exit(1);}
	sprintf(lj, "/proc/%d/mem", ipid);
	handle = open(lj, O_RDWR);
	if (handle == 0)
	{
		puts("获取mem失败!");
		exit(1);
	}

}


ssize_t process_v(pid_t __pid, const struct iovec* __local_iov, unsigned long __local_iov_count,
                  const struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite) {
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool pvm(int ipid , void* address, void* buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (ipid < 0) {
        return false;
    }

    ssize_t bytes = process_v(ipid, local, 1, remote, 1, 0, iswrite);
    return bytes == size;
}
bool vm_readv(long int address, void *buffer, size_t size) {
	return pvm(ipid, reinterpret_cast<void *>(address), buffer, size, false);
}

/* 写内存 */
bool vm_writev(long int address, void *buffer, size_t size) {
	return pvm(ipid, reinterpret_cast<void *>(address), buffer, size, true);
}



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
//分辨率获取函数
int remoteSwitch()
{
    FILE *fp = fopen("/sdcard/x.log", "r");
	FILE *fp1 = fopen("/sdcard/y.log", "r");
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
	puts("获取游戏PID失败");
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

/* 读整数型内存 */
int getDword(long int addr) {
	long int var[1] = { 0 };
	vm_readv(addr, var, 4);
	return var[0];
}

/* 读浮点型内存 */
float getFloat(long int addr) {
	float var[1] = { 0 };
	vm_readv(addr, var, 4);
	return var[0];
}

long int getZZ(long int addr)
{
	long int var[1] = { 0 };
	vm_readv(addr, var, 4);
	return var[0];
}

long int getword(long int addr)
{
	long int var[1] = { 0 };
	vm_readv(addr, var, 2);
	return var[0];
}

 void WriteFloat(long int addr, float value)
{

char lj[64];
int handle;

sprintf(lj,"/proc/%d/mem",ipid);
handle=open(lj,O_RDWR);
lseek64(handle,0,SEEK_SET);
pwrite(handle,&value,4,addr);
close(handle);

	
}

 void WriteDword(long int addr, int value)
{
	
char lj[64];
int handle;

sprintf(lj,"/proc/%d/mem",ipid);
handle=open(lj,O_RDWR);
lseek64(handle,0,SEEK_SET);
pwrite(handle,&value,4,addr);
close(handle);
	
}



// 获取名字

//using namespace std;


struct Vector2A {
	float X;
	float Y;

	Vector2A() {
		this->X = 0;
		this->Y = 0;
	}

	Vector2A(float x, float y) {
		this->X = x;
		this->Y = y;
	}
};

struct Vector3A {
	float X;
	float Y;
	float Z;

	Vector3A() {
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3A(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

};

struct Vector4A {
	float x;
	float y;
	float h;
	float w;

	Vector4A() {
		this->x = 0;
		this->y = 0;
		this->h = 0;
		this->w = 0;
	}


	Vector4A (float x, float y, float h, float w) {
		this->x = x;
		this->y = y;
		this->h = h;
		this->w = w;
	}
};

enum Mode {
	InitMode = 1,
	ESPMode = 2,
	HackMode = 3,
	StopMode = 4,
};

struct Request {
	int Mode;
	int ScreenWidth;
	int ScreenHeight;
	bool isSpeed;
	bool isBone;
	bool isItems;
	bool isVehicle;
};

struct BoneData {
	Vector2A Head;
	Vector2A Chest;
	Vector2A Pelvis;
	Vector2A Left_Shoulder;
	Vector2A Right_Shoulder;
	Vector2A Left_Elbow;
	Vector2A Right_Elbow;
	Vector2A Left_Wrist;
	Vector2A Right_Wrist;
	Vector2A Left_Thigh;
	Vector2A Right_Thigh;
	Vector2A Left_Knee;
	Vector2A Right_Knee;
	Vector2A Left_Ankle;
	Vector2A Right_Ankle;
};

struct PlayerData {
	bool isAI;
	int TeamID;
	float Health;
	float Angle;
	int State;
	float AimedYawDiff;
	float Distance;
	char PlayerName[32];
	Vector4A Location;
	Vector2A RadarLocation;
	Vector2A Head;
	Vector2A Chest;
	Vector2A Pelvis;
	Vector2A Left_Shoulder;
	Vector2A Right_Shoulder;
	Vector2A Left_Elbow;
	Vector2A Right_Elbow;
	Vector2A Left_Wrist;
	Vector2A Right_Wrist;
	Vector2A Left_Thigh;
	Vector2A Right_Thigh;
	Vector2A Left_Knee;
	Vector2A Right_Knee;
	Vector2A Left_Ankle;
	Vector2A Right_Ankle;
};

struct ItemData {
	float Distance;
	char ItemName[32];
	Vector2A Location;
};

struct Response {
	bool Success; //状态
	bool isHall; //大厅
	int PlayerCount; //人
	int GrenadeCount; //投掷物
	int VehicleCount; //车
	int ItemsCount; //物资
	int AirBoxCount; //空投盒子飞机
	PlayerData Players[MaxPlayerCount];
	ItemData Grenades[MaxGrenadeCount];
	ItemData Vehicle[MaxVehicleCount];
	ItemData Items[MaxItemsCount];
	ItemData AirBox[MaxAirBoxCount];
};

struct FMatrix {
	float M[4][4];
};

struct Quat {
	float X;
	float Y;
	float Z;
	float W;
};

struct FTransform {
	Quat Rotation;
	Vector3A Translation;
	float chunk;
	Vector3A Scale3D;
};

struct MAPS {
	long int addr;
	long int taddr;
	int type;
	struct MAPS *next;
};

typedef struct MAPS *PMAPS;

#define LEN sizeof(struct MAPS)


/* ------ 相关数据计算 ------ */
/*人物骨骼*/
FMatrix TransformToMatrix(FTransform transform) {
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}


/* 计算3D坐标距离 */
float getDistance(float zx, float zy, float zz, float dx, float dy, float dz) {
	float x,y,z;
	x = zx - dx;
	y = zy - dy;
	z = zz - dz;
	return (float)(sqrt(x * x + y * y + z * z));
}

Vector2A WorldToScreen(Vector3A obj , float matrix[16]) {
	float ViewW = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
	float x = px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y = py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;
	return Vector2A(x, y);
}

Vector2A WorldToScreen2(Vector3A obj , float matrix[16]) {
	float ViewW = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
	if (ViewW < 0.01) {
		return Vector2A(3100, 3100);
	}
	float x = px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y = py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;
	return Vector2A(x,y);
}

Vector3A MarixToVector(FMatrix matrix) {
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2) {
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FTransform getBone(long int addr) {
	FTransform transform;
	vm_readv(addr, &transform, 4 * 11);
	return transform;
}




















