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
#include "32位读取.h"

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

// 判断文件是否存在
bool IsPathExist(const std::string & path)
{
	if (access(path.c_str(), 0) == F_OK)
	{
		return true;
	}
	return false;
}

// 转码配置
int gs;
long int Uworld;
float MatrixValue[16] = { 0 };
long int selfteam;
FILE *fw;
FILE *wz;
float fww, fwww, wzz, wzzz,tz;
int yy,kh;
double hs =
	180 / 3.1415926535897932385, result, y_result, minhp, x[4], z[4], xr[4], zxjl, khjc1, khpd,khjc;
long int kjpdzz, sbxy;
int zdjl = 100;
float z_x, z_z, z_y,zm_x, zm_z, zm_y,obj_x, obj_z, obj_y;
float camear_z,r_x,r_y,r_w,X,Y,W,H;
float camear_zz,r_xx,r_yy,r_ww;
UTF8 Name[200] = "";
int hp ;
int team;
double m,jl;
int rs;
int aimstate;
int grjj;
int grjjj;
int selfzbc;
int zjzz;
//int khjc;
long int zjzzz,zjzbc;


char *strrpc2(char *str, char *oldstr, char *newstr)
{
	char bstr[strlen(str)];
	memset(bstr, 0, sizeof(bstr));

	for (int i = 0; i < strlen(str); i++)
	{
		if (!strncmp(str + i, oldstr, strlen(oldstr)))
		{
			strcat(bstr, newstr);
			i += strlen(oldstr) - 1;
		}
		else
		{
			strncat(bstr, str + i, 1);
		}
	}

	strcpy(str, bstr);
	return str;
}


char *Unicode(int to16d)
{
	char *retres;
	char to16s[64];
	int to16zd;
	char str[12];
	retres = (char *)malloc(sizeof(short));
	sprintf(to16s, "%p", to16d);
	strrpc2(to16s, "ffff", "");
	to16zd = strtol(to16s, NULL, 16);
	wchar_t wstr[] = { to16zd, 0 };
	setlocale(LC_ALL, "");
	wcstombs(str, wstr, sizeof(str) / sizeof(char));
	strcpy(retres, str);
	free(retres);
	return retres;
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





int pd = getPID("com.sofunny.Sausage");




if (pd == 0)
	{
		puts("辅助未启动!");
	exit(1);
	}

 ipid = getPID("com.sofunny.Sausage");

char RangeValue[10240];
	char AdjustH[10240];



	if (ipid == 0)
	{
		puts("获取进程失败!");
		exit(1);
	}

	// 分辨率获取
	
	remoteSwitch();

	char oneso[] = "libunity.so";
	long int libone = get_module_base(ipid, oneso);
	char twoso[] = "libil2cpp.so";
	long int libtwo = get_module_base(ipid, twoso);
char threeso[] = "libtersafe.so";
	long int libthree = get_module_base(ipid, threeso);



	//std::string path = "/sdcard/Dream";

	FILE *F;
	while ((F = fopen("/sdcard/停止", "r")) == NULL)	// 文件存在则停止循环
	{
	    fw = fopen("/sdcard/fw", "r");
		fscanf(fw, "%f", &fww);
		fwww = fww;
	    fclose(fw);
	    wz = fopen("/sdcard/wz", "r");
		fscanf(wz, "%f", &wzz);
	    wzzz = wzz;
	    fclose(wz);
	    
	     
	    
		 //printf("自瞄范围:%f\n",fwww);
		// usleep(2 * 1000000);

		memset(RangeValue, 0, 10240);
		memset(AdjustH, 0, 10240);

		
		// 矩阵
	    long int xmatrix = getZZ(libone+0x1132630);
 	    long int Matrix = getZZ(getZZ(xmatrix + +0x154) + 0x74) + 0xDC;
//		printf("矩阵地址=%lx\n", Matrix);
		// 坐标
		char aaa[40960];
		char b[30720];
		        long int Uworld = getZZ(libone + 0x1110F60);//游戏世界
        long int Uleve  =getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(Uworld+0x4) + 0x4) + 0x8) + 0x18) + 0x2C) +
				0x8);
		long int count = getDword(Uleve + 0xC);	// 
		long int DXSZ = Uleve + 0x14;	// 对象列表
		long int Uleve1 = getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(libtwo + 0x39E0F0) + 0x30) + 0x1F0) + 0x44) + 0x138) + 0x8) + 0x10) + 0xF4)	;	// 自身结构体
		sbxy = getZZ(getZZ(getZZ(getZZ(getZZ(getZZ(libtwo + 0x35C394) + 0x1c) + 0x4c) + 0x44) + 0x10) + 0x0) + 0x0;
	    long int zszb = getZZ(Uleve+0x10);
		

        long int zmAddrx = getZZ(getZZ(getZZ(getZZ(libtwo+0x112C4)+0x35C)+0x5C)+0xF0)+0x58;
        long int zmAddry = getZZ(getZZ(getZZ(getZZ(libtwo+0x112C4)+0x35C)+0x5C)+0xF0)+0x5C;
     
                 
         long int  xy1 =  getZZ(libtwo+0x112C4) ;           
         long int  xy2 =  getZZ(xy1+0x35C) ;      
         long int xy3  =getZZ(xy2+0x5C);
           
        long int xy4=getZZ(xy3+0xF0) ;     
                 
   //      kh = getDword(xy4+0x2CC);

	//	long int zsxx=getZZ(Uleve1 + 0xF4);																												
	//	printf("世界地址=%lx\n", DXSZ);
		
		
		;	// 自身队伍编号

memset(aaa, 0, 2048);
    rs = 0;
		for (int i = 0; i < 16; i++)	/* 矩阵 */
		{

			float fhjz = getFloat(Matrix + i * 4);
			matrix[i] = fhjz;
		}




		

		for (int ii = 0; ii < count; ii++)
		{
			long int objaddr = getZZ(DXSZ + 4 * ii);	// 遍历数量次数

		
			
		
			if (objaddr== 0)
				{
					continue;
					}
					
			//敌人坐标指针层
		long int zbc= getZZ(objaddr+ 0x30C);
			
			
			// 敌人坐标
			 obj_x = getFloat(zbc + 0x34);
			obj_z = getFloat(zbc + 0x38);
			obj_y = getFloat(zbc + 0x3C);
			
			//printf("%f\n", obj_x);
       	if (wzz == 0 )
		{
			
			tz = 0.95; 
			}
			
				
			if (wzz == 1 )
			{
			
			tz =  0.75;
			
			}
				
			if (wzz == 2 )
			{
			
			tz = 0.35;
			
			}
			
			//tz = 0.7;
		
		//	printf("%f\n", tz);
			 zm_x = getFloat(zmAddrx);
		//	 zm_z = getFloat(zbc + 0x38) + tz ;
			 zm_y = getFloat(zmAddry);
			zjzbc= getZZ(zszb+ 0x30C);
		     z_x = getFloat(zjzbc + 0x34);
			 z_z = getFloat(zjzbc + 0x38) ;
			 z_y = getFloat(zjzbc + 0x3C);
			
			/* 人物信息指针层
			long int xx1 = getZZ(objaddr + 0xF4);	// 动作身体武器
	    	 team = getDword(xx1 + 0x154);
	        //long int team1 = getDword(Uleve1 + 0x154);
			long int pdzj= getDword(xx1 + 0x130);//判断自己指针 1为自己 0为他人
			if  ( pdzj == 1 )
			{
			zjzzz = getZZ(DXSZ + 4 * ii);
			long int selfzz = getZZ(objaddr + 0xF4);
			 selfteam = getDword(selfzz + 0x154); //自己阵营地址
			 zjzbc= getZZ(getZZ(getZZ(objaddr+ 0xF0) + 0x10) + 0x8);
			*/
		
				// 队伍编号
			//printf("判断自己:%lx\n", pdzj);
			// 人物血量
			 hp = getFloat(objaddr + 0xA4)+0xB0;

			char Name[200] = "";
			for (int c = 0; c < 6; c++)
			{
				short Namecode = getword(getZZ(xx1 + 0x14) + 0x2C + c * 2);
			strcat(Name, Unicode(Namecode));
			}

			camear_z = matrix[3] * obj_x + matrix[7] * obj_z + matrix[11] * obj_y + matrix[15];	// 相机Z
		 m = camear_z / 1;
			if (dy == 0)
			{
				
				
				if (hp <= 0)
				{
					continue;
				}

	
				if (hp > 100 )
				{
					continue;
				}
				
			

			}
			

				 r_x =
				px + (matrix[0] * obj_x  + matrix[4] * (obj_z) + matrix[8] * obj_y +
					  matrix[12]) / camear_z * px;

			r_y =
				py - (matrix[1] * obj_x + matrix[5] * (obj_z + 0.5) + matrix[9] * obj_y +
					  matrix[13]) / camear_z * py;

			 r_w =
				py - (matrix[1] * obj_x + matrix[5] * (obj_z + 1.5) + matrix[9] * obj_y +
					  matrix[13]) / camear_z * py;


X=r_x - (r_y - r_w) / 4;
Y=r_y;
W=(r_y - r_w) / 2;
H=r_y - r_w;

	camear_zz = matrix[3] * zm_x + matrix[7] * zm_z + matrix[11] * zm_y + matrix[15];	// 相机Z
			 
			   if (camear_zz < 0){
           continue;
           }
			
			 jl = camear_zz / 1;
			
			 r_xx =
				px + (matrix[0] * zm_x  + matrix[4] * (zm_z) + matrix[8] * zm_y +
					  matrix[12]) / camear_zz * px;

			 r_yy =
				py - (matrix[1] * zm_x + matrix[5] * (zm_z + 0.5) + matrix[9] * zm_y +
					  matrix[13]) / camear_zz * py;

			 r_ww =
				py - (matrix[1] * zm_x + matrix[5] * (zm_z + 1.5) + matrix[9] * zm_y +
					  matrix[13]) / camear_zz * py;	  
		

			sprintf(b, "%f,%f,%f,%f,%f,%d,%d,%s\n",
		        	X,	// f 1
					Y,		// f 2
					W,	// f 3
					H,	// f 4
					m,			// f 5
					hp,			// d 6
					1,
					Name		// s 10
				);
			strcat(aaa, b);
			
			float jlx, jly, jlz;
				if (zm_x >= z_x && zm_y >= z_y)
				{

					jlx = -z_x + zm_x;
					jly = zm_y - z_y;
					result = 90 - atan(jly / jlx) * hs;

				}
				if (zm_x >= z_x && zm_y <= z_y)
				{
					jlx = -z_x + zm_x;
					jly = -zm_y + z_y;
					result = 180 - atan(jlx / jly) * hs;

				}
				if (zm_x <= z_x && zm_y >= z_y)
				{

					jlx = z_x - zm_x;
					jly = zm_y - z_y;
					result = 360 - atan(jlx / jly) * hs;
				}

				if (zm_x <= z_x && zm_y <= z_y)
				{
					jlx = z_x - zm_x;
					jly = -zm_y + z_y;
					result = 270 - atan(jly / jlx) * hs;
				}
				if (zm_z <= z_z)
				{
					jlz = z_z - zm_z;
					y_result = atan(jlz / m) * hs;
					yy = 1;
				}
				else
				{
					jlz = -z_z + zm_z;
					y_result = atan(jlz / m) * hs;
					yy = 2;
				}
				x[0] = sin(y_result / 2 / hs) * cos(result / 2 / hs);
				if (result > 180)
				{
					x[0] = -x[0];
				}
				x[1] = sin(result / 2 / hs) * cos(y_result / 2 / hs);
				x[3] = sqrt(0.5 + 0.5 * cos(y_result / hs) - pow(x[1], 2));
				x[2] = sqrt(0.5 - 0.5 * cos(y_result / hs) - pow(x[0], 2));
				if (result < 180)
				{
					if (yy == 2)
					{
						x[0] = -1 * x[0];
					}
					else
					{
						x[2] = -1 * x[2];
					}
				}
				else
				{
					if (yy == 2)
					{
						x[3] = -1 * x[3];
					}
					else
					{
						x[0] = -1 * x[0];
						x[2] = -1 * x[2];
						x[3] = -1 * x[3];
					}
				}

	

  
		
	
		
					
		
	//	printf("傻逼",kh);
		

		
		
		
			  	float pmjl = sqrt(pow(r_x - px, 2) + pow(r_y - py, 2));

				
			if ( pmjl > fwww )	// 瞄准准星距离 
				{
					continue;
				}
				rs++;
				if (rs == 1 || pmjl <= zxjl)
				{
					zxjl = pmjl;
					for (int a = 0; a < 4; a++)
					{
						z[a] = x[a];
					}
				}
			}
		int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
		write(fd, aaa, sizeof(aaa));	// 写入文本
		close(fd);
		usleep(2000);

			
			for (int ii = 0; ii < 4; ii++)
			{
				xr[ii] = z[ii];

	}
	
		
		
		//	puts("0");
		
	
	
//  kh = getDword(kjpdzz + 0x13c); // 开火 0为不开火 1为开火
	    //	printf("测试:%p\n", kjpdzz);
	    
	    FILE *hh=fopen("/sdcard/自瞄关","r");//读取文件是否存在
if (hh == NULL)//如果读取不到文件,则执行↓
{
//puts("开启自瞄");
  kh = getDword(xy4+0x2CC);

//kh =getDword( zjzzz + 0x4c);
khjc = getFloat (zjzzz +0x44c);
if (khjc != khjc1 || kh == 1 || kh==257)//开火判断 ||狙击开镜判断
		{
		//	puts("1");
			if (rs > 0)
				for (int iii = 0; iii < 4; iii++)
				{
				//	puts("2");
					long int zbzm = zjzzz  + 0x234 + 0x4 * iii;	// 设置地址
					WriteFloat(zbzm, xr[iii]);
				//	printf("测试:%p\n", xr[iii]);
          
				}
		}	
		khjc1 = khjc;
		usleep(50);
		
}

	
			
			
	
		
		}
		

	
	
	return 0;
	
	
	
	

}


			
			
		