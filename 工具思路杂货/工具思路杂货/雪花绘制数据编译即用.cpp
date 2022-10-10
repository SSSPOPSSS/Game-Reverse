




#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <dirent.h>
#include <thread>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Memory.h"
#include "utf.h"
#include "Structs.h"
#include <string.h>
#include <locale>
#include <string>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <locale>
#include <string>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//#include "t3data.h"
#include <string.h>
#include<cmath>
#define PI 3.141592653589793238
#define DWORD 4
#define FLOAT 16
using namespace std;




/* 游戏变量 */
int target_pid = 0, ResNum = 0, SelfTeamID = 0;
long int oneself = 0;
float px = 0, py = 0;
long int dz = 0;
long int dz2 = 0;
long int dz3 = 0;
long int dz4 = 0;
long int dz5 = 0;
long int dz6 = 0;
PMAPS pMap_A = nullptr, pMap_P = nullptr, pMap_ALL = nullptr;
long int MatrixAddress = (long int)malloc(8 * 10);
long int myaddrs = (long int)malloc(8 * 10);

long int uWorldAddress = (long int)malloc(8 * 10);
long int ActorAddress = (long int)malloc(8 * 10);
long int UleveAddress = (long int)malloc(8 * 10);
long int GnameAddress = (long int)malloc(8 * 10);
long int libUE4 = (long int)malloc(8 * 10);
float MatrixValue[16] = { 0 };
int ResInfo[2] = { 0 };

bool isForegRound = true;

/* ------ 内存读写 ------ */




struct D2DVector
{
	float X;
	float Y;
};
/* 读内存 */
bool vm_readv(long int address, void *buffer, size_t size)
{
	return pvm(target_pid, reinterpret_cast < void *>(address), buffer, size, false);
}

/* 写内存 */
bool vm_writev(long int address, void *buffer, size_t size)
{
	return pvm(target_pid, reinterpret_cast < void *>(address), buffer, size, true);
}

/* 获取进程PID */
/* 
   int find_pid(const char *szPro) { struct dirent *ptr = nullptr; char
   cmdline[256], szName[256]; DIR *dir = opendir("/proc"); if (nullptr != dir)
   { while ((ptr = readdir(dir)) != nullptr) { if ((strcmp(ptr->d_name, ".") == 
   0) || (strcmp(ptr->d_name, "..") == 0)) continue; if (ptr->d_type != DT_DIR)
   continue; sprintf(cmdline, "/proc/%s/cmdline", ptr->d_name); FILE *fp =
   fopen(cmdline, "r"); if (nullptr != fp) { fgets(szName, sizeof(szName), fp);
   if (strcmp(szName, szPro) == 0) { break; } fclose(fp); } } } if (readdir(dir) 
   == nullptr) { return 0; } closedir(dir); return atoi(ptr->d_name); }

 */
int find_pid(const char *packageName)
{
	int id = -1;
	DIR *dir;
	FILE *fp;
	char filename[32];
	char cmdline[256];
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


/* 读取模块入口地址 */
long int get_module_base(int pid, const char *module_name)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[32];
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
long int getint(long int addr)
{
	long int var[1] = { 0 };
	vm_readv(addr, var, 4);
	return var[0];
}

/* 读浮点型内存 */
float getfloat(long int addr)
{
	float var[1] = { 0 };
	vm_readv(addr, var, 4);
	return var[0];
}

/* 读文本型内存 */
char *getText(long int address, unsigned int size)
{
	char *data = new char[size];
	vm_readv(address, data, size);
	return data;
}

/* 写浮点型内存 */
void writefloat(long int addr, float data)
{
	vm_writev(addr, &data, 4);
}

/* ------ 相关数据计算 ------ */
/* 计算3D坐标距离 */
float getDistance(float zx, float zy, float zz, float dx, float dy, float dz)
{
	float x, y, z;
	x = zx - dx;
	y = zy - dy;
	z = zz - dz;
	return (float)(sqrt(x * x + y * y + z * z));
}

Vector2A WorldToScreen(Vector3A obj, float matrix[16])
{
	float ViewW = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
	float x =
		px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y =
		py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;
	return Vector2A(x, y);
}

Vector2A WorldToScreen2(Vector3A obj, float matrix[16])
{
	float ViewW = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
	if (ViewW < 0.01)
	{
		return Vector2A(3100, 3100);
	}
	float x =
		px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y =
		py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;
	return Vector2A(x, y);
}

Vector3A MarixToVector(FMatrix matrix)
{
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)
{
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)
{
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

FTransform getBone(long int addr)
{
	FTransform transform;
	vm_readv(addr, &transform, 4 * 11);
	return transform;
}

/* ------ 线程处理 ------ */

D2DVector rotateCoord(float angle, float objectRadar_x, float objectRadar_y)
{
	D2DVector radarCoordinate;
	float s = sin(angle * PI / 180);
	float c = cos(angle * PI / 180);
	radarCoordinate.X = objectRadar_x * c + objectRadar_y * s;
	radarCoordinate.Y = -objectRadar_x * s + objectRadar_y * c;
	return radarCoordinate;
}


int remoteSwitch()
{
	// 分辨率获取
	FILE *fp = fopen("/sdcard/x.log", "r");
	// 分辨率，格式:x,w
	FILE *fp1 = fopen("/sdcard/y.log", "r");
	// 状态栏
	if (fp == NULL || fp1 == NULL)
	{
		px = 2340.0 / 2;
		py = 1080.0 / 2;
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
	;
}

int StopPID(int pid)
{
	char ml[64];
	sprintf(ml, "kill -STOP %d", pid);

	system(ml);
	// 退出没有root的进程
}

// 恢复某个应用
int ResetFromStopPID(int pid)
{
	char ml[64];
	sprintf(ml, "kill -CONT %d", pid);

	system(ml);
}



void createDataList()
{
	for (;;)
	{

		char aaa[12000] = "";
		char b[500];
		// int scwq;
		{
			/* 资源信息 */
			UleveAddress = getint(getint(uWorldAddress + 0xA8)+0x700);	// 指针
			ActorAddress = UleveAddress;	// 指针
			ResNum = getint(getint(uWorldAddress + 0xA8)+ 0x704);	// 指针 
		//	printf("%d\n",	ActorAddress);
			/* 矩阵数据 */
			memset(MatrixValue, 0, 16);
			vm_readv(MatrixAddress + 0x6D0, MatrixValue, 4 * 16);	// 矩阵指针
			long int Matrix1 = MatrixAddress + 0x6D0;
			/* 自身坐标 */
			long int SelfInfoCorrect = Matrix1 - 0x14;	/* 从x开始读 */
			Vector3A SelfInfo;
			vm_readv(SelfInfoCorrect + 0x4, &SelfInfo, sizeof(SelfInfo));	// 自身坐标

			// SelfTeamID = getint(oneself + 0x63C);
			for (int i = 0; i < ResNum; i++)
			{

				// D2DVector angleJudgement; // 旋转距离
				/* 对象指针 */
				long int Tzz = getint(ActorAddress + 0xC * i);

				float pdzz = getfloat(Tzz + 0x168);
				if (pdzz == 0)
				{
					continue;
				}
				long int ObjectPointer = getint(Tzz + 0xC8);	// 人物数组
				
				if (getfloat(ObjectPointer + 0x1CC4) != 479.5)
				{
					continue;
				}
				
				int teamID = getint(ObjectPointer + 0x63C);
				int dy = getint(ObjectPointer + 0x196C);
				if (teamID < 0 || teamID > 110 || dy != 0)
				{
					continue;
				}

				/* if (getfloat(ObjectPointer + 0x1CC4) != 479.5) { continue;
				   } */ 
					/* 坐标指针 */
				long int RootCompont = Tzz;//getint(ObjectPointer + 0x158);

				Vector3A ObjInfo;
				vm_readv(RootCompont + 0x160, &ObjInfo, sizeof(ObjInfo));	// 对象坐标

				/* 坐标转换 */
				float camear_z =
					MatrixValue[3] * ObjInfo.X + MatrixValue[7] * ObjInfo.Y +
					MatrixValue[11] * ObjInfo.Z + MatrixValue[15];
				float r_x =
					px + (MatrixValue[0] * ObjInfo.X + MatrixValue[4] * ObjInfo.Y +
						  MatrixValue[8] * ObjInfo.Z + MatrixValue[12]) / camear_z * px;
				float r_y =
					py - (MatrixValue[1] * ObjInfo.X + MatrixValue[5] * ObjInfo.Y +
						  MatrixValue[9] * (ObjInfo.Z - 10) + MatrixValue[13]) / camear_z * py;
				float r_w =
					py - (MatrixValue[1] * ObjInfo.X + MatrixValue[5] * ObjInfo.Y +
						  MatrixValue[9] * (ObjInfo.Z + 180) + MatrixValue[13]) / camear_z * py;
				/* 判断人机 */
				int isAI = getint(ObjectPointer + 0x2D4) != 0 ? 1 : 0;

				/* 人物名字 */
				long int NamePointer = getint(ObjectPointer + 0x5F8);
				UTF8 PlayerName[32] = "";
				UTF16 buf16[16] = { 0 };
				vm_readv(NamePointer, buf16, 28);
				Utf16_To_Utf8(buf16, PlayerName, 28, strictConversion);

				/* 血量 */
				float hp = getfloat(ObjectPointer + 0x864);

				/* 对象距离 */
				float Distance =
					getDistance(ObjInfo.X, ObjInfo.Y, ObjInfo.Z, SelfInfo.X, SelfInfo.Y,
								SelfInfo.Z) / 100;

				if (Distance > 500 || Distance < 1)
				{
					continue;
				}


				/* 对象距离超过游戏可视距离不输出 */
				long int MeshOffset = getint(ObjectPointer + 0x334);
				/* 阵列指针 */
				long int MeshAddress = MeshOffset + 0x150;
				/* 骨骼指针 */
				long int BoneAddress = getint(MeshOffset + 0x584) + 0x30;
				FTransform meshtrans = getBone(MeshAddress);
				FMatrix c2wMatrix = TransformToMatrix(meshtrans);
				// Head
				FTransform headtrans = getBone(BoneAddress + 5 * 48);
				FMatrix boneMatrix = TransformToMatrix(headtrans);
				Vector3A relLocation = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
				relLocation.Z += 7;
				Vector2A Head = WorldToScreen(relLocation, MatrixValue);
				// Chest
				FTransform chesttrans = getBone(BoneAddress + 4 * 48);
				FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
				Vector3A relLocation1 = MarixToVector(MatrixMulti(boneMatrix1, c2wMatrix));
				Vector2A Chest = WorldToScreen(relLocation1, MatrixValue);
				// Left Sholder
				FTransform lshtrans = getBone(BoneAddress + 11 * 48);
				FMatrix boneMatrix2 = TransformToMatrix(lshtrans);
				Vector3A relLocation2 = MarixToVector(MatrixMulti(boneMatrix2, c2wMatrix));
				Vector2A Left_Shoulder = WorldToScreen(relLocation2, MatrixValue);
				// Right Shoulder
				FTransform rshtrans = getBone(BoneAddress + 32 * 48);
				FMatrix boneMatrix3 = TransformToMatrix(rshtrans);
				Vector3A relLocation3 = MarixToVector(MatrixMulti(boneMatrix3, c2wMatrix));
				Vector2A Right_Shoulder = WorldToScreen(relLocation3, MatrixValue);
				// Left Elbow
				FTransform lelbtrans = getBone(BoneAddress + 12 * 48);
				FMatrix boneMatrix4 = TransformToMatrix(lelbtrans);
				Vector3A relLocation4 = MarixToVector(MatrixMulti(boneMatrix4, c2wMatrix));
				Vector2A Left_Elbow = WorldToScreen(relLocation4, MatrixValue);
				// Right Elbow
				FTransform relbtrans = getBone(BoneAddress + 33 * 48);
				FMatrix boneMatrix5 = TransformToMatrix(relbtrans);
				Vector3A relLocation5 = MarixToVector(MatrixMulti(boneMatrix5, c2wMatrix));
				Vector2A Right_Elbow = WorldToScreen(relLocation5, MatrixValue);
				// Left Wrist
				FTransform lwtrans = getBone(BoneAddress + 63 * 48);
				FMatrix boneMatrix6 = TransformToMatrix(lwtrans);
				Vector3A relLocation6 = MarixToVector(MatrixMulti(boneMatrix6, c2wMatrix));
				Vector2A Left_Wrist = WorldToScreen(relLocation6, MatrixValue);
				// Right Wrist
				FTransform rwtrans = getBone(BoneAddress + 62 * 48);
				FMatrix boneMatrix7 = TransformToMatrix(rwtrans);
				Vector3A relLocation7 = MarixToVector(MatrixMulti(boneMatrix7, c2wMatrix));
				Vector2A Right_Wrist = WorldToScreen(relLocation7, MatrixValue);
				// Pelvis
				FTransform Lchesttrans = getBone(BoneAddress + 1 * 48);
				FMatrix LboneMatrix1 = TransformToMatrix(Lchesttrans);
				Vector3A LrelLocation1 = MarixToVector(MatrixMulti(LboneMatrix1, c2wMatrix));
				Vector2A Pelvis = WorldToScreen(LrelLocation1, MatrixValue);
				// Left thigh
				FTransform Llshtrans = getBone(BoneAddress + 52 * 48);
				FMatrix LboneMatrix2 = TransformToMatrix(Llshtrans);
				Vector3A LrelLocation2 = MarixToVector(MatrixMulti(LboneMatrix2, c2wMatrix));
				Vector2A Left_Thigh = WorldToScreen(LrelLocation2, MatrixValue);
				// Right thigh
				FTransform Lrshtrans = getBone(BoneAddress + 56 * 48);
				FMatrix LboneMatrix3 = TransformToMatrix(Lrshtrans);
				Vector3A LrelLocation3 = MarixToVector(MatrixMulti(LboneMatrix3, c2wMatrix));
				Vector2A Right_Thigh = WorldToScreen(LrelLocation3, MatrixValue);
				// Left Knee
				FTransform Llelbtrans = getBone(BoneAddress + 53 * 48);
				FMatrix LboneMatrix4 = TransformToMatrix(Llelbtrans);
				Vector3A LrelLocation4 = MarixToVector(MatrixMulti(LboneMatrix4, c2wMatrix));
				Vector2A Left_Knee = WorldToScreen(LrelLocation4, MatrixValue);
				// Right Knee
				FTransform Lrelbtrans = getBone(BoneAddress + 57 * 48);
				FMatrix LboneMatrix5 = TransformToMatrix(Lrelbtrans);
				Vector3A LrelLocation5 = MarixToVector(MatrixMulti(LboneMatrix5, c2wMatrix));
				Vector2A Right_Knee = WorldToScreen(LrelLocation5, MatrixValue);
				// Left Ankle
				FTransform Llwtrans = getBone(BoneAddress + 54 * 48);
				FMatrix LboneMatrix6 = TransformToMatrix(Llwtrans);
				Vector3A LrelLocation6 = MarixToVector(MatrixMulti(LboneMatrix6, c2wMatrix));
				Vector2A Left_Ankle = WorldToScreen(LrelLocation6, MatrixValue);
				// Right Ankle
				FTransform Lrwtrans = getBone(BoneAddress + 58 * 48);
				FMatrix LboneMatrix7 = TransformToMatrix(Lrwtrans);
				Vector3A LrelLocation7 = MarixToVector(MatrixMulti(LboneMatrix7, c2wMatrix));
				Vector2A Right_Ankle = WorldToScreen(LrelLocation7, MatrixValue);

				// ResetFromStopPID(target_pid);

				/* 对象距离超过游戏可视距离输出 */
				sprintf(b, "%.2f,%.2f,%.2f,%.2f,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", r_x - (r_y - r_w) / 4,	// x
						r_y,	// y
						(r_y - r_w) / 2,	// 3.w
						r_y - r_w,	// h
						(int)Distance - 2,	// 距离
						hp,		// 血量
						isAI,	// 人几
						teamID,	// 队伍
						(int)Head.X, (int)Head.Y,	// 头部
						(int)Chest.X, (int)Chest.Y,	// 胸部
						(int)Pelvis.X, (int)Pelvis.Y,	// 盆骨
						(int)Left_Shoulder.X, (int)Left_Shoulder.Y,	// 左肩
						(int)Right_Shoulder.X, (int)Right_Shoulder.Y,	// 右肩
						(int)Left_Elbow.X, (int)Left_Elbow.Y,	// 左手肘
						(int)Right_Elbow.X, (int)Right_Elbow.Y,	// 右手肘
						(int)Left_Wrist.X, (int)Left_Wrist.Y,	// 左手腕
						(int)Right_Wrist.X, (int)Right_Wrist.Y,	// 右手腕
						(int)Left_Thigh.X, (int)Left_Thigh.Y,	// 左大腿
						(int)Right_Thigh.X, (int)Right_Thigh.Y,	// 右大腿
						(int)Left_Knee.X, (int)Left_Knee.Y,	// 左膝盖
						(int)Right_Knee.X, (int)Right_Knee.Y,	// 右膝盖
						(int)Left_Ankle.X, (int)Left_Ankle.Y,	// 左脚腕
						(int)Right_Ankle.X, (int)Right_Ankle.Y,	// 右脚腕 
						PlayerName
						// 
					);

				strcat(aaa, b);

			}
		}
		sprintf(b, "0,0");
		strcat(aaa, b);
		int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
			write(fd, aaa, sizeof(aaa));	// 写入文本
		usleep(5);
	}
}


int Memorywrite(char *value, int type, long int offset)
{
	char mem[33];
	snprintf(mem, sizeof(mem), "/proc/%d/mem", target_pid);
	int fd = open(mem, O_WRONLY);
	if (fd < 0)
	{
		return -1;
	}
	if (type == DWORD)
	{
		int buf = atoi(value);
		pwrite(fd, &buf, type, offset);
	}
	if (type == FLOAT)
	{
		float buf = atof(value);
		pwrite(fd, &buf, type, offset);
	}
	close(fd);
	return 0;
}

long getaddr(const char *so, char *ncd)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[32];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", target_pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, so) && strstr(line, ncd))
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



char *host = "w5.eydata.net";	// 接口域名 不用动
											// 只能使用此域名
char *ver = "1.0";				// 接口调用码 调用地址 /?后面

char *dq(char *path)
{
	FILE *fp;
	if ((fp = fopen(path, "r")) == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	char *str;
	str = (char *)malloc(filesize);
	rewind(fp);
	fread(str, 1, filesize, fp);
	str[filesize] = '\0';
	fclose(fp);
	return str;
}

int main(int argc, char *argv[])
{

	

	printf("成功");
	target_pid = find_pid("com.tencent.tmgp.pubgmhd");	// 选择游戏包名
	if (target_pid == 0)
	{
		printf("游戏未运行\n");
		return 0;
	}
	// 
	// char sml[64];
	// system("pkill -f xg_vip");


	remoteSwitch();

	/* UE4模块入口地址 */
	libUE4 = get_module_base(target_pid, "libUE4.so");	// 读取基址so
	/* 游戏矩阵地址 */
	MatrixAddress = getint(getint(libUE4 + 123267360) + 0x68);	// 矩阵基址偏移
	// myaddrs = getint(libUE4 + 0x7020170);
	uWorldAddress = getint(libUE4 + 0x7306230);	// 世界读取基址

	/* 循环读取应用 */
	std::thread t3(createDataList);

	// ResetFromStopPID(target_pid);
	for (;;)
	{
		sleep(1);
	}

	// pclose(fp);
	return 0;

}