#include<stdio.h>
int main()
{
char nameout[20];
FILE *fout;
int i,n=100;
for (i=1;i<=n;i++)
{
sprintf(nameout,"POP\\%d.txt",i);
fout=fopen(nameout,"w");
fprintf(fout,"%d",i);
fclose(fout);
printf("手机的文件好像多了");
}
return 0;
}

//会改参数就改
