#include "MemoryTools.h"
int main(int argc,char **argv)
{
killGG();
int gs;//用来存储数据个数
void *jg;
PACKAGENAME *bm="com.popcap.pvz";
BypassGameSafe();
SetSearchRange(C_ALLOC);
puts("开始内存搜索...");
MemorySearch(bm,"16777216;50;20;256:269",&gs,DWORD);
printf("共搜索%d个数据\n",gs);
MemorySearch(bm,"50",&gs,DWORD);
printf("共改善%d个数据\n",gs);
Print();
MemoryWrite(bm,"99999",0,DWORD);
puts("修改成功");puts("修改完成");
ClearResults();
}
