#include "MemoryTools.h"
int main(int argc,char **argv){killGG();int gs;void *jg;
PACKAGENAME *bm="Pop.con";
BypassGameSafe();SetSearchRange(C_ALLOC);
puts("数据搜索中...");
MemorySearch(bm,"7E06614558",&gs,FLOAT);
printf("搜索到%d个数据\n",gs);
MemoryOffset(bm,"7E06614544",-20,&gs,FLOAT);
printf("将修改%d个数据\n",gs);Print();
MemoryWrite(bm,"7E06614530",-40,FLOAT);
puts("测试");puts("修改完成");
ClearResults();}