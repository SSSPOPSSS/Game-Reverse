#include "MemoryTools.h"
int main(int argc,char **argv)
{
killGG();
int gs;//用来存储数据个数
void *jg;
PACKAGENAME *bm="游戏包名";//软件包名
//注意这里要用指针,就是那个*号,不可以去掉

BypassGameSafe();
SetSearchRange(C_ALLOC);//搜索内存
MemorySearch(bm,"主特码",&gs,FLOAT);//搜索主特征码
printf("共搜索%d个数据\n",gs);
MemoryOffset(bm,"副特征码",0x60,&gs,FLOAT);//主特征码偏移到这里副特征码和偏移
MemoryOffset(bm,"1",0x70,&gs,FLOAT);
MemoryOffset(bm,"0.02999999933",0x78,&gs,FLOAT);
printf("共偏移%d个数据\n",gs);
Print();
MemoryWrite(bm,"-0.4",0x74,FLOAT);//修改主特征码偏移到要改的数据及偏移
printf("共修改%d个数据\n",gs);
ClearResults();
}