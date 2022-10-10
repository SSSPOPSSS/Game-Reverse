#include <stdio.h>
#include <time.h>
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
#include <codecvt>				// 对接端口
#include <cstring>
#include "64位读取.h"

#include <pthread.h>			// for thread(多线程)
#include <arpa/inet.h>
#define SERVER_PORT 9999
#include <unistd.h>

int dy = 0;
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>



float MatrixValue[16] = { 0 };
int ai;
//#include



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




void getCharacterName(UTF8 * transcoding, long int addr)	// 传入指针
{
	int classname;
	int m = 0;
	UTF8 buf88[256] = "";
	long int namepy = getZZ(addr + 0x850);
	UTF16 buf16[34] = {
		0
	};
	int hex[2] = {
		0
	};
	for (int i = 0; i < 4; i++)
	{
		classname = getZZ(namepy + i * 4);
		hex[0] = (classname & 0xfffff000) >> 16;
		hex[1] = classname & 0xffff;
		buf16[m] = hex[1];
		buf16[m + 1] = hex[0];
		m += 2;
	}
	Utf16_To_Utf8(buf16, buf88, sizeof(buf88), strictConversion);
	sprintf(transcoding, "%s", buf88);
}


void Connectcommunication()
{
    int clientSocket = 0;			// 通讯辨识
	char sendbuf[200];
	char recvbuf[200];
	int iDataNum;
	int i = 0;
	int ret = 1;
	struct sockaddr_in serverAddr;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket创建失败");
		exit(1);
	}
	else
	{
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(SERVER_PORT);
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("未连接到服务端");
		exit(1);
	}
	else
	{
		printf("成功连接服务端\n");
	}
}



int main(int argc, char **argv)
{

   Connectcommunication();

//Connectcommunication();

FILE *hh=fopen("/sdcard/Dream","r");//读取文件是否存在
if (hh == NULL)//如果读取不到文件,则执行↓
{
puts("无法获取验证文件");
exit(1);
//命令一：rebootsystem();//重启系统(手机)
//命令二：remove("填写文件或系统所在路径");//删除某个文件或系统
//命令三：uninstallapk("填写要卸载的软件包名");//静默卸载软件
}




int pd = getPID("com.tencent.dreamvip");




if (pd == 0)
	{
		puts("辅助未启动!");
	exit(1);
	}

      



     puts("程序开始");
	//读取进程
	 ipid = getPID("com.tencent.tmgp.pubgmhd");
	 
	 if (ipid == 0)
	{
		puts("获取游戏进程失败!");
	exit(1);
	}

	// 分辨率获取
	remoteSwitch();
	
	// 读取基址
	puts("开始基址遍历\n");
	char mname[] = "libUE4.so";	// libUE4模块
	long int libbase = get_module_base(ipid, mname);//获取libUE4入口模块地址
	// 读取矩阵
	long int Matrix = getZZ(getZZ(libbase + 0xAD36EB0) + 0x98) + 0x320  ;// 游戏矩阵
	printf("矩阵地址=%lx\n", Matrix);
	
			
	FILE *F;
	while ((F = fopen("/sdcard/停止", "r")) == NULL)	// 文件存在则停止循环
	{

			char aaa[60720] = "";	// 为变量申请内存
		char b[10000];
        /*资源信息列表*/
       /*资源信息列表*/
                                             
		
		long int Uworld = getZZ(libbase + 0xAD58338);	// 读取游戏世界
		long int Uleve = getZZ(Uworld + 0x30);	// 读取游戏世界
		
			// Ul
		long int arrayaddr = getZZ(Uleve + 0xa0);	// 对象阵列
		long int count = getDword(Uleve + 0xa8);	// 遍历数量
		long int oneself = getZZ(getZZ(getZZ(getZZ(Uworld + 0x38) + 0x78) + 0x30) + 0x3F0);	// 自身对象
	
		long int team = getDword(oneself + 0x8b0);	// 自身队伍编号
		
		memset(MatrixValue, 0, 16);//初始化矩阵数组
		vm_readv(Matrix, MatrixValue, 4 * 16);	// 矩阵指针
		
		
		//printf("世界地址:%d\n",zjdw);





		// 获取矩阵
		for (int i = 0; i < 16; i++)
		{
			float matrixaddr = getFloat(Matrix + i * 4);
			matrix[i] = matrixaddr;
		}
		
		//printf("%d\n", count);

		// 获取坐标 遍历数目
		for (int i = 0; i < count; i++)
		{
			
			
			
			//对象指针
			long int objaddrzz = getZZ(arrayaddr + 0x8 * i);	// 遍历数量次数

  if (objaddrzz == 0x0000000000 || objaddrzz == 0 || objaddrzz == 0x000 )
			{
				continue;
			}				

			// 去除非人物坐标
			float wzzb = getFloat(objaddrzz + 0x2524);		
			if (wzzb != 479.5)
			{
				continue;
			}					// 不等于479.5则不输出坐标 
         
			// 队伍编号
			int zy = getDword(objaddrzz + 0x8b0);
				
		long int dy = getZZ(objaddrzz + 0x20b8);	// 自身队伍编号
		
			    if (dy != 0)
			{
				continue;
			}				

			
    if (zy < 0  || zy == 996)
			{
				continue;
			}				

			// 去除自身和队友方框
			// 去除方框(队伍号相同则不显示)
			if (team == zy )		// 不输出自己和队友坐标
			{
				continue;
			}
            //坐标指针
			long int object = getZZ(objaddrzz + 0x1D8);	// 对象地址指针
			
            /* 对象坐标 */
				Vector3A ObjInfo;
				vm_readv(object + 0x1C0, &ObjInfo, sizeof(ObjInfo));
			// 自身坐标
			float z_x = getFloat(Matrix - 0x10);
			float z_y = getFloat(Matrix - 0xC);
			float z_z = getFloat(Matrix - 0x8);
			
			
		//	printf("%f\n", z_z);




		float d_x = getFloat(object + 0x1C0);
			float d_y = getFloat(object + 0x1C4);
			float d_z = getFloat(object + 0x1C8) ;
	//	printf("%f\n", d_x);
			
			

			// 人物距离
			float m = sqrt(pow(d_x - z_x, 2) + pow(d_y - z_y, 2) + pow(d_z - z_z, 2)) * 0.01;
			if (m > 400)
			{
				continue;
			}					// 大于400米不显示

			// 计算人物矩阵
			float camear_r = matrix[3] * d_x + matrix[7] * d_y + matrix[11] * d_z + matrix[15];
			float r_x =px + (matrix[0] * d_x + matrix[4] * d_y + matrix[8] * d_z +
					  matrix[12]) / camear_r * px;
			float r_y = py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z - 5) +
					  matrix[13]) / camear_r * py;
			float r_w = py - (matrix[1] * d_x + matrix[5] * d_y + matrix[9] * (d_z + 205) +
			         matrix[13]) / camear_r * py;

			// 玩家名字
			UTF8 Name[32] = "";
			getCharacterName(Name, objaddrzz);

			// 健康血量
			int hp = getFloat(objaddrzz + 0xB98);
			
			
		//	printf("血量:%f\n", hp);

			// 人机识别
			int rj = getDword(objaddrzz + 0x3D0);
			
			if (rj == 1 ){
			ai =0;
			}
			if (rj == 0 ){
			ai =1;
			}
			
			
		   
			

			// 人物动作
			int dz = getDword(objaddrzz + 0xD68);

			// 倒地血量
			float dx = 0;
			
			//坐标
			float X=r_x - (r_y - r_w) / 4;
			
			float Y=r_y;
			
			float W=(r_y - r_w) / 2;
			
			float H=r_y - r_w;
			
			
			
			
				/* 阵列偏移 */
					long int MeshOffset = getZZ( objaddrzz+ 0x470);//yes
					/* 阵列指针 */
					long int MeshAddress = MeshOffset + 0x1b0;//yes
					/* 骨骼指针 */
					long int BoneAddress = getZZ(MeshOffset + 0x6E0) + 0x30;//骨骼指针
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
				
			
			
			
			
			
			sprintf(b, "%f,%f,%f,%f,%f,%d,%d,%d,%d,%f,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			        X,	// 1.x
					Y,		// 2.y
					W,	// 3.w
					H,	// 4.h
					m,			// 5.玩家距离
					hp,			// 6.人物血量
					ai,		// 7.人机判断
					dz,			// 8.人物动作
					zy,			// 9.队伍阵营
					dx,			// 10.倒地血量
					Name,		// 11.玩家名字					
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
					(int)Right_Ankle.X, (int)Right_Ankle.Y  // 右脚腕					
					);
			strcat(aaa, b);
		
			}
		
			 
			int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
		write(fd, aaa, sizeof(aaa));	// 写入文本
			close(fd);
            usleep(2000);             
	}
	
}

 
  	