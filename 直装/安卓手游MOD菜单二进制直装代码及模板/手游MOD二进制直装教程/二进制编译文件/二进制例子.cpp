#include "MemoryTools.h"
int main(int argc,char **argv)
{
killGG();
int gs;//用来存储数据个数
void *jg;
PACKAGENAME *bm="com.nidong.cmswat.m4399";//软件包名
//注意这里要用指针,就是那个*号,不可以去掉

BypassGameSafe();
SetSearchRange(C_ALLOC);
MemorySearch(bm,"3.58732407e-43",&gs,FLOAT);//搜索内存
printf("共搜索%d个数据\n",gs);
MemoryOffset(bm,"1.26116862e-44",-4,&gs,FLOAT);
MemoryOffset(bm,"0.00100000005",8,&gs,FLOAT);
MemoryOffset(bm,"0.00100000005",18,&gs,FLOAT);
printf("共偏移%d个数据\n",gs);
Print();
MemoryWrite(bm,"9999",48,FLOAT);
printf("共修改%d个数据\n",gs);
ClearResults();
}