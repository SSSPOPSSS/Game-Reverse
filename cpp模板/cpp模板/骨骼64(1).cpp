#include "Momery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>

float MatrixValue[16] = { 0 };
int isapkrunning(PACKAGENAME * bm)
{
DIR *dir = NULL;
struct dirent *ptr = NULL;
FILE *fp = NULL;
char filepath[50];
char filetext[128];
dir = opendir("/proc/");
if (dir != NULL)
{
while ((ptr = readdir(dir)) != NULL)
{
if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
continue;
if (ptr->d_type != DT_DIR)
continue;
sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
fp = fopen(filepath, "r");
if (NULL != fp)
{
fgets(filetext, sizeof(filetext), fp);
if (strcmp(filetext, bm) == 0)
{
closedir(dir);
return 1;
}
fclose(fp);
}
}
}
closedir(dir);
return 0;
}


void Residue()
{
	while (true)
	{
		char *c_route = "/sdcard/shouhun/b.log";
		int fd2 = open(c_route, O_WRONLY | O_APPEND);
		write(fd2, "0,0,0;", 5);
		close(fd2);
		usleep(1500 * 1000);// 10ms
	}
}








int main(int argc, char **argv)
{

	FILE *fp = fopen("/sdcard/shouhun/x", "r");
	FILE *fp1 = fopen("/sdcard/shouhun/y", "r");
	if (fp == NULL || fp1 == NULL)// 如果没有读取到分辨率文件,则设置以下分辨
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
//读取进程
ipid = getPID("com.tencent.tmgp.pubgmhd");
//分辨率获取
// 读取基址
char mname[] = "libUE4.so";// libUE4模块
long int libbase = get_module_base(ipid, mname);//获取libUE4入口模块地址
puts("程序开始");
puts("开始基址遍历\n");

// 读取矩阵
long int Matrix = getZZ_64(getZZ_64(libbase + 0xABA5030) + 0x98) + 0x320;// 游戏矩阵
printf("矩阵地址=%lx\n", Matrix);
std::thread removedResidue(Residue);
FILE * F;
while ((F = fopen("/sdcard/shouhun/stop", "r")) == NULL)// 文件存在则停止循环
{
if(getPID("com.tencent.tmgp.pubgmhd")!=-1){
char aaa[6800] = "";// 为变量申请内存
char b[500];
/*资源信息列表*/
long int Uworld = getZZ_64(libbase + 0xABC6498);//游戏世界
long int Uleve = getZZ_64(Uworld + 0x30);// Uleve
long int arrayaddr = getZZ_64(Uleve + 0xb0);// 对象阵列
long int count = getDword(Uleve + 0xb8);// 遍历数量
//
long int oneself = getZZ_64(getZZ_64(getZZ_64(getZZ_64(Uworld + 0x38) + 0x78) + 0x30) + 0x3F0);// 自身对象
long int team = getDword(oneself + 0x8b0);// 自身队伍编号
memset(MatrixValue, 0, 16);//初始化矩阵数组
vm_readv(Matrix, MatrixValue, 4 * 16);//矩阵指针
//获取矩阵
for (int i = 0; i < 16; i++)
{
float matrixaddr = getFloat(Matrix + i * 4);
matrix[i] = matrixaddr;
}
//获取坐标
for (int i = 0; i < count; i++)
{
//对象指针
long int objaddrzz = getZZ_64(arrayaddr + 8 * i);//遍历数量
//去除物资
float wzzb = getFloat(objaddrzz + 0x24E4);
if (wzzb != 479.5)
{
continue;
}

//队伍编号(去除自身队友)
int zy = getDword(objaddrzz + 0x8b0);
if (team == zy)
{
continue;
}
//printf("玩家名字:%x\n",objaddrzz);
//坐标指针
long int object = getZZ_64(objaddrzz + 0x1D8);// 对象地址指针
//对象坐标
Vector3A ObjInfo;
vm_readv(object + 0x1C0, &ObjInfo, sizeof(ObjInfo));
//自身坐标
float z_x = getFloat(Matrix - 0x10);
float z_y = getFloat(Matrix - 0xC);
float z_z = getFloat(Matrix - 0x8);
// 敌人坐标
float d_x = getFloat(object + 0x1C0);
float d_y = getFloat(object + 0x1C4);
float d_z = getFloat(object + 0x1C8);
//人物距离(大于2000米不显示)
float m = sqrt(pow(d_x - z_x, 2) + pow(d_y - z_y, 2) + pow(d_z - z_z, 2)) * 0.01;
if (m > 2000)
{
continue;
}
// 计算人物矩阵
float camear_r = matrix[3] * d_x + matrix[7] * d_y + matrix[11] * d_z + matrix[15];
float r_x =px + (matrix[0] * d_x + matrix[4] * d_y + matrix[8] * d_z +
matrix[12]) / camear_r * px;
float r_y = py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z - 5) +
matrix[13]) / camear_r * py;
float r_w = py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z + 205) +
matrix[13]) / camear_r * py;
//玩家名字
UTF8 Name[32] = "";
getCharacterName_64(Name, objaddrzz);
//健康血量
float hp = getFloat(objaddrzz + 0xB98);
// 人机识别
int ai = getDword(objaddrzz + 0x3D0);
// 人物动作
int dz = 0;
// 倒地血量
float dx = 0;
//人物坐标
float X=r_x - (r_y - r_w) / 4;
float Y=r_y;
float W=(r_y - r_w) / 2;
float H=r_y - r_w;
//阵列偏移
long int MeshOffset = getZZ_64(objaddrzz + 0x470);
//阵列指针
long int MeshAddress = MeshOffset + 0x1b0;
//骨骼指针
long int BoneAddress = getZZ_64(MeshOffset + 0x6D0) + 0x30;
FTransform meshtrans = getBone(MeshAddress);//yes
FMatrix c2wMatrix = TransformToMatrix(meshtrans);//yes
//Head
FTransform headtrans = getBone(BoneAddress + 5 * 48);
FMatrix boneMatrix = TransformToMatrix(headtrans);
Vector3A relLocation = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
relLocation.Z += 7;
Vector2A Head = WorldToScreen(relLocation, MatrixValue);
//箱箱
FTransform chesttrans = getBone(BoneAddress + 4 * 48);
FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
Vector3A relLocation1 = MarixToVector(MatrixMulti(boneMatrix1 , c2wMatrix));
Vector2A Chest = WorldToScreen(relLocation1 , MatrixValue);
//左肩
FTransform lshtrans = getBone(BoneAddress + 11 * 48);
FMatrix boneMatrix2 = TransformToMatrix(lshtrans);
Vector3A relLocation2 = MarixToVector(MatrixMulti(boneMatrix2 , c2wMatrix));
Vector2A Left_Shoulder = WorldToScreen(relLocation2 , MatrixValue);
//右肩
FTransform rshtrans = getBone(BoneAddress + 32 * 48);
FMatrix boneMatrix3 = TransformToMatrix(rshtrans);
Vector3A relLocation3 = MarixToVector(MatrixMulti(boneMatrix3 , c2wMatrix));
Vector2A Right_Shoulder = WorldToScreen(relLocation3 , MatrixValue);
//Left Elbow
FTransform lelbtrans = getBone(BoneAddress + 12 * 48);
FMatrix boneMatrix4 = TransformToMatrix(lelbtrans);
Vector3A relLocation4 = MarixToVector(MatrixMulti(boneMatrix4 , c2wMatrix));
Vector2A Left_Elbow = WorldToScreen(relLocation4 , MatrixValue);
//Right Elbow
FTransform relbtrans = getBone(BoneAddress + 33 * 48);
FMatrix boneMatrix5 = TransformToMatrix(relbtrans);
Vector3A relLocation5 = MarixToVector(MatrixMulti(boneMatrix5 , c2wMatrix));
Vector2A Right_Elbow = WorldToScreen(relLocation5 , MatrixValue);
//Left Wrist
FTransform lwtrans = getBone(BoneAddress + 63 * 48);
FMatrix boneMatrix6 = TransformToMatrix(lwtrans);
Vector3A relLocation6 = MarixToVector(MatrixMulti(boneMatrix6 , c2wMatrix));
Vector2A Left_Wrist = WorldToScreen(relLocation6 , MatrixValue);
//Right Wrist
FTransform rwtrans = getBone(BoneAddress + 62 * 48);
FMatrix boneMatrix7 = TransformToMatrix(rwtrans);
Vector3A relLocation7 = MarixToVector(MatrixMulti(boneMatrix7 , c2wMatrix));
Vector2A Right_Wrist = WorldToScreen(relLocation7 , MatrixValue);
//Pelvis
FTransform Lchesttrans = getBone(BoneAddress + 1 * 48);
FMatrix LboneMatrix1 = TransformToMatrix(Lchesttrans);
Vector3A LrelLocation1 = MarixToVector(MatrixMulti(LboneMatrix1 , c2wMatrix));
Vector2A Pelvis = WorldToScreen(LrelLocation1 , MatrixValue);
//Left thigh
FTransform Llshtrans = getBone(BoneAddress + 52 * 48);
FMatrix LboneMatrix2 = TransformToMatrix(Llshtrans);
Vector3A LrelLocation2 = MarixToVector(MatrixMulti(LboneMatrix2 , c2wMatrix));
Vector2A Left_Thigh = WorldToScreen(LrelLocation2 , MatrixValue);
//Right thigh
 FTransform Lrshtrans = getBone(BoneAddress + 56 * 48);
FMatrix LboneMatrix3 = TransformToMatrix(Lrshtrans);
Vector3A LrelLocation3 = MarixToVector(MatrixMulti(LboneMatrix3 , c2wMatrix));
Vector2A Right_Thigh = WorldToScreen(LrelLocation3 , MatrixValue);
//Left Knee
FTransform Llelbtrans = getBone(BoneAddress + 53 * 48);
FMatrix LboneMatrix4 = TransformToMatrix(Llelbtrans);
Vector3A LrelLocation4 = MarixToVector(MatrixMulti(LboneMatrix4 , c2wMatrix));
Vector2A Left_Knee = WorldToScreen(LrelLocation4 , MatrixValue);
//Right Knee
FTransform Lrelbtrans = getBone(BoneAddress + 57 * 48);
FMatrix LboneMatrix5 = TransformToMatrix(Lrelbtrans);
Vector3A LrelLocation5 = MarixToVector(MatrixMulti(LboneMatrix5 , c2wMatrix));
Vector2A Right_Knee = WorldToScreen(LrelLocation5 , MatrixValue);
//Left Ankle
FTransform Llwtrans = getBone(BoneAddress + 54 * 48);
FMatrix LboneMatrix6 = TransformToMatrix(Llwtrans);
Vector3A LrelLocation6 = MarixToVector(MatrixMulti(LboneMatrix6 , c2wMatrix));
Vector2A Left_Ankle = WorldToScreen(LrelLocation6 , MatrixValue);
//Right Ankle
FTransform Lrwtrans = getBone(BoneAddress + 58 * 48);
FMatrix LboneMatrix7 = TransformToMatrix(Lrwtrans);
Vector3A LrelLocation7 = MarixToVector(MatrixMulti(LboneMatrix7 , c2wMatrix));
Vector2A Right_Ankle = WorldToScreen(LrelLocation7 , MatrixValue);
sprintf(b, "%f,%f,%f,%f,%f,%f,%d,%d,%f,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;\n",
X,// 1.x
Y,// 2.y
W,// 3.w
H,// 4.h
m,// 5.玩家距离
hp,// 6.人物血量
ai,// 7.人机判断
zy,// 9.队伍阵营
dx,// 10.倒地血量
Name,// 11.玩家名字
(int)Head.X, (int)Head.Y,// 头部
(int)Chest.X, (int)Chest.Y,// 胸部
(int)Pelvis.X, (int)Pelvis.Y,// 盆骨
(int)Left_Shoulder.X, (int)Left_Shoulder.Y,// 左肩
(int)Right_Shoulder.X, (int)Right_Shoulder.Y,// 右肩 
(int)Left_Elbow.X, (int)Left_Elbow.Y,// 左手肘
(int)Right_Elbow.X, (int)Right_Elbow.Y,// 右手肘 
(int)Left_Wrist.X, (int)Left_Wrist.Y,// 左手腕
(int)Right_Wrist.X, (int)Right_Wrist.Y,// 右手腕
(int)Left_Thigh.X, (int)Left_Thigh.Y,// 左大腿
(int)Right_Thigh.X, (int)Right_Thigh.Y,// 右大腿
(int)Left_Knee.X, (int)Left_Knee.Y,// 左膝盖
(int)Right_Knee.X, (int)Right_Knee.Y,// 右膝盖
(int)Left_Ankle.X, (int)Left_Ankle.Y,// 左脚腕
(int)Right_Ankle.X, (int)Right_Ankle.Y  // 右脚腕
);
strcat(aaa, b);
}
int fd = open("/sdcard/shouhun/b.log", O_WRONLY | O_CREAT);
write(fd, aaa, sizeof(aaa));//写入文本
close(fd);
usleep(0.01);
}
else
{
system("touch /sdcard/shouhun/stop");
}
}
system("rm -rf /sdcard/shouhun/stop");
}