
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
#include "Tools/Memory.h"
#include "Tools/utf.h"
#include "Tools/Structs.h"
// #define SERVER_PORT 2000 // 对接端口
#include <cstring>
#include <algorithm>

using namespace std;

/* 游戏变量 */
int target_pid = 0, ResNum = 0, SelfTeamID = 0;
float px = 0, py = 0;
PMAPS pMap_A = nullptr, pMap_P = nullptr, pMap_ALL = nullptr;
long int MatrixAddress = (long int)malloc(8 * 10);
long int uWorldAddress = (long int)malloc(8 * 10);
long int ActorAddress = (long int)malloc(8 * 10);
long int UleveAddress = (long int)malloc(8 * 10);
long int GnameAddress = (long int)malloc(8 * 10);
long int libUE4 = (long int)malloc(8 * 10);
float MatrixValue[16] = { 0 };
int ResInfo[2] = { 0 };

bool isForegRound = true;

/* ------ 内存读写 ------ */

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
int find_pid(const char *szPro)
{
	struct dirent *ptr = nullptr;
	char cmdline[256], szName[256];
	DIR *dir = opendir("/proc");
	if (nullptr != dir)
	{
		while ((ptr = readdir(dir)) != nullptr)
		{
			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
				continue;
			if (ptr->d_type != DT_DIR)
				continue;
			sprintf(cmdline, "/proc/%s/cmdline", ptr->d_name);
			FILE *fp = fopen(cmdline, "r");
			if (nullptr != fp)
			{
				fgets(szName, sizeof(szName), fp);
				if (strcmp(szName, szPro) == 0)
				{
					break;
				}
				fclose(fp);
			}
		}
	}
	if (readdir(dir) == nullptr)
	{
		return 0;
	}
	closedir(dir);
	return atoi(ptr->d_name);
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

void Get_State()
{
	for (;;)
	{
		char cmdline[256], szName[256];
		sprintf(cmdline, "/proc/%d/oom_adj", target_pid);
		FILE *fp = fopen(cmdline, "r");
		if (nullptr != fp)
		{
			fgets(szName, sizeof(szName), fp);
			if (atoi(szName) != 11)
			{
				isForegRound = true;
			}
			else
			{
				isForegRound = false;
			}
			fclose(fp);
		}
		sleep(1);
	}
}

void Thread_State()
{
	for (;;)
	{
		std::thread gg(Get_State);
		gg.join();
	}
}

int remoteSwitch()
{
	// 分辨率获取
	FILE *fp = fopen("/sdcard/x", "r");
	// 分辨率，格式:x,w
	FILE *fp1 = fopen("/sdcard/y", "r");
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

long int *ActorArray = (long *)malloc(8 * 100);
void createDataList()
{
	for (;;)
	{
		char aaa[18000] = "";
		char b[500];
		/* 资源信息 */
		uWorldAddress = getint(libUE4 + 0x75A656C);	// 世界读取基址
		UleveAddress = uWorldAddress;	// 指针
		/* 资源名信息 */
		// GnameAddress = getint(libUE4 + 0x73C6094);//名字基址
		memset(ResInfo, 0, 2);
		vm_readv(UleveAddress + 0x198, ResInfo, 4 * 2);	// 世界二级指针
		ActorAddress = ResInfo[0] - 0xFFFFFFFF00000000;	// level上一级
		ResNum = ResInfo[1];
		/* 矩阵数据 */
		memset(MatrixValue, 0, 16);
		vm_readv(MatrixAddress + 0x6D0, MatrixValue, 4 * 16);	// 矩阵指针
		long int NetDriver = getint(uWorldAddress + 0x24);	// 世界+24
		long int ServerConnection = getint(NetDriver + 0x64);
		long int PlayerController = getint(ServerConnection + 0x20);
		// long int MyAddress = getint(PlayerController +
		// 0x324);//自己地址
		long int MyAddress = getint(getint(getint(getint(uWorldAddress + 36) + 100) + 32) + 0x334);
		Vector3A SelfInfo;
		vm_readv(MatrixAddress + 0x6C0, &SelfInfo, sizeof(SelfInfo));
  		int p = 0;
		for (int gs = 0; gs < ResNum; gs++)
		{
			long int player_target = getint(getint(ActorAddress + 0x4* gs) + 0xc8);
			float role = getfloat(player_target + 0x1cc4);
			if (role != 479.5)
			{
				continue;
			}
			ActorArray[p] = player_target;
			p++;
			
		}
		sort(ActorArray, ActorArray + p, less < int >());
		int k = unique(ActorArray, ActorArray + p) - ActorArray;
    
		for (int i = 0; i < k; i++)
		{

			/* 对象指针 */
		   	long int ObjectPointer = ActorArray[i];	// 人物数组
		    
		    	
		    	int Troop = getint(ObjectPointer + 0x63c);
				if (Troop < 0 || Troop > 100)
				{
					continue;
				}

				int teammate;
				if (ObjectPointer == MyAddress)
					teammate = Troop;
				if (teammate == Troop)
				{
					continue;
				}
				/* 坐标指针 */
				long int RootCompont = getint(ObjectPointer + 0x334);	// 敌人坐标指针
				/* 对象坐标 */
				Vector3A ObjInfo;
				vm_readv(RootCompont + 0x160, &ObjInfo, sizeof(ObjInfo));	// 对象坐标
				/* 玩家名称 */
				long int NamePointer = getint(ObjectPointer + 0x5f8);	// 
				UTF8 PlayerName[32] = "";
				UTF16 buf16[16] = { 0 };
				vm_readv(NamePointer, buf16, 28);
				Utf16_To_Utf8(buf16, PlayerName, 28, strictConversion);
				/* 血量信息 */
				float HelathInfo[2] = { 0 };
				vm_readv(ObjectPointer + 0x864, HelathInfo, sizeof(HelathInfo));
				float Health = HelathInfo[0] / HelathInfo[1] * 100;
				/* 判断人机 */
				int isAI = getint(ObjectPointer + 0x2D4);

				/* 对象距离 */
				float Distance =
					getDistance(ObjInfo.X, ObjInfo.Y, ObjInfo.Z, SelfInfo.X, SelfInfo.Y,
								SelfInfo.Z) / 100;
				
				//动作
				int State = getint(ObjectPointer + 0x9b0);



				/* 坐标转换 */
				float camear_z =
					MatrixValue[3] * ObjInfo.X + MatrixValue[7] * ObjInfo.Y +
					MatrixValue[11] * ObjInfo.Z + MatrixValue[15];
				float r_x =
					px + (MatrixValue[0] * ObjInfo.X + MatrixValue[4] * ObjInfo.Y +
						  MatrixValue[8] * ObjInfo.Z + MatrixValue[12]) / camear_z * px;
				float r_y =
					py - (MatrixValue[1] * ObjInfo.X + MatrixValue[5] * ObjInfo.Y +
						  MatrixValue[9] * (ObjInfo.Z - 5) + MatrixValue[13]) / camear_z * py;
				float r_w =
					py - (MatrixValue[1] * ObjInfo.X + MatrixValue[5] * ObjInfo.Y +
						  MatrixValue[9] * (ObjInfo.Z + 250) + MatrixValue[13]) / camear_z * py;


				/* 阵列偏移自己地址 */
				long int MeshOffset = getint(ObjectPointer + 0x334);

				long int MeshAddress = MeshOffset + 0x150;

				long int BoneAddress = getint(MeshOffset + 0x578) + 0x30;
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
				sprintf(b, "%f,%f,%f,%f,%d,%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;", r_x - (r_y - r_w) / 4,	// x
						r_y,	// y
						(r_y - r_w) / 2,	// w
						r_y - r_w,	// h
						(int)Distance,	// 距离
						(int)Health,	// 血量 
						isAI,	// 人机判断
						Troop,	// 队编 
						PlayerName,	// 名字
						State,		// 动作 
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
						(int)Right_Ankle.X, (int)Right_Ankle.Y	// 右脚腕
					);
				strcat(aaa, b);
			}
		

		sprintf(b, "无数据");
		char *path = "/sdcard/b.log";
		strcat(aaa, b);

		int fd = open(path, O_RDWR | O_CREAT);
		write(fd, aaa, sizeof(aaa));
		close(fd);
		usleep(200);

	}
}


void Thread_Data()
{
	for (;;)
	{
		std::thread gg(createDataList);
		gg.join();
	}
}

int main(int argc, char *argv[])
{
	/* Root权限获取 */

	char ml[64];
	sprintf(ml, "su -c %s", *argv);
	if (getuid() != 0)
	{
		system(ml);
	}

	/* 获取游戏进程 */
	target_pid = find_pid("com.tencent.tmgp.pubgmhd");	// 选择游戏包名
	if (target_pid == 0)
	{
		printf("游戏未运行!\n");
		// return 0;
	}

	printf("运行中!\n");

	remoteSwitch();

	/* UE4模块入口地址 */
	libUE4 = get_module_base(target_pid, "libUE4.so");	// 读取基址so
	/* 游戏矩阵地址 */
	MatrixAddress = getint(getint(libUE4 + 0x758E920) + 0x68);	// 矩阵基址cb内存 
																// 
	// 
	// 
	// 
	std::thread t2(Thread_State);

	/* 循环读取应用 */
	std::thread t3(Thread_Data);

	for (;;)
	{
		FILE *F;
		if ((F = fopen("/sdcard/停止绘制.log", "r")) != NULL)
		{
			sleep(1);
			printf("已结束运行");
			system("rm -rf /sdcard/停止绘制.log");
			exit(1);
		}
	}
	return 0;

}
