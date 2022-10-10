#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h
#include <stdlib.h>
int main(int argc, char** argv){
    puts("\033[31m 正在运行中 \33[0m");
    // system("rm -rf /data/app/com.netease.aceracer.m4399*");
    //这种方法不需要遍历但是要root
    /*
    for循环int是给他分配一个内存i＝100是如果i＝100则循环当然他初始值就是100当然循环啦*/    
    for(int i=100;i=100;i++)
    {
        //printf("A B 5 4 6 8");
        //第二种方法不需要root但是要遍历才能删除文件夹
        remove("/data/data/com.netease.aceracer/shared_prefs/ZG5zLmNhY2hl.xml");
    //  sprintf("chmod 000 /data/data/com.netease.aceracer/files");
     system("chmod 000 /data/data/com.netease.aceracer/shared_prefs");


        // puts("\033[31m A B C 5 2 0 0 \33[0m");
   
     }
    return 0;
}
