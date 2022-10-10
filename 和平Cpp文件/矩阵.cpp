#include "MemoryTools.h"
int main(int argc,char **argv){killGG();int gs;void *jg;
PACKAGENAME *bm="ppppp";
BypassGameSafe();SetSearchRange(A_ANONMYOUS);
puts("数据搜索中...");
MemorySearch(bm,"7DD3D09F08",&gs,FLOAT);
printf("搜索到%d个数据\n",gs);
MemoryOffset(bm,"7DD3D09EDC",-44,&gs,FLOAT);
printf("将修改%d个数据\n",gs);Print();
MemoryWrite(bm,"7DD3D09EE8",-30,FLOAT);
puts("矩阵");puts("修改完成");
ClearResults();}