#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
typedef long int ADDRESS;typedef char PACKAGENAME;int ipid;int getPID(char bm[64]);int WriteAddress_DWORD(PACKAGENAME *bm,ADDRESS addr, int value);int WriteAddress_FLOAT(char*bm,ADDRESS addr, float value);int killGG();int killprocess(PACKAGENAME *bm){int pid=getPID(bm);	if (pid == 0)	{	return -1;	}	char ml[32];	sprintf(ml,"kill %d",pid);	system(ml);	return 0;}int killGG(){	DIR *dir=NULL;	DIR *dirGG=NULL;	struct dirent *ptr=NULL;	struct dirent *ptrGG=NULL;	char filepath[256];		char filetext[128];		dir = opendir("/data/data");			int flag=1;	if (dir != NULL)	{		while (flag && (ptr = readdir(dir)) != NULL)			{		if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))				continue;			if (ptr->d_type != DT_DIR)				continue;			sprintf(filepath, "/data/data/%s/files", ptr->d_name);				dirGG = opendir(filepath);				if (dirGG != NULL)			{				while ((ptrGG = readdir(dirGG)) != NULL)				{					if ((strcmp(ptrGG->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))				        continue;       if (ptrGG->d_type != DT_DIR)			        	continue;	        if (strstr(ptrGG->d_name,"GG"))	        {			        	int pid;			        	pid = getPID(ptr->d_name);			        	if (pid == 0)			        	    continue;			        	else			        		killprocess(ptr->d_name);			        }				}			}			}	}	closedir(dir);	closedir(dirGG);	return 0;};int isapkrunning(char *bm);long int getZZ(int pid,long int addr);bool snowpd(long int addr);long int get_module_base(int pid,const char*module_name){FILE*fp;long addr=0;char*pch;char filename[32];char line[1024];snprintf(filename,sizeof(filename),"/proc/%d/maps",pid);fp=fopen(filename,"r");if (fp!=NULL){while (fgets(line,sizeof(line),fp)){if (strstr(line,module_name)){pch=strtok(line,"-");addr=strtoul(pch,NULL,16);if (addr==0x8000)addr=0;break;}}fclose(fp);}return addr;}long int getZZ(int pid,long int addr){long int var[4]={0};int handle;char lj[256];memset(var,0,4);sprintf(lj,"/proc/%d/mem",pid);handle=open(lj,O_RDWR);lseek(handle,0,SEEK_SET);pread64(handle,var,4,addr);close(handle);return var[0];}int getPID(PACKAGENAME *PackageName){DIR *dir=NULL;struct dirent *ptr=NULL;FILE *fp=NULL;char filepath[256];char filetext[128];dir=opendir("/proc");if (NULL != dir){while ((ptr=readdir(dir))!=NULL){if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))continue;if (ptr->d_type != DT_DIR)continue;sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);fp=fopen(filepath, "r");if (NULL!=fp){fgets(filetext,sizeof(filetext),fp);if (strcmp(filetext,PackageName)==0){break;}fclose(fp);}}}if (readdir(dir) == NULL){return 0;}closedir(dir);return atoi(ptr->d_name);}int WriteAddress_DWORD(PACKAGENAME *bm,ADDRESS addr, int value){pid_t pid=getPID(bm);if (pid==0){puts("can not get pid");return 0;}char lj[64];int handle;sprintf(lj, "/proc/%d/mem", pid);handle=open(lj, O_RDWR);lseek(handle, 0, SEEK_SET);pwrite64(handle, &value, 4, addr);close(handle);return 0;}int WriteAddress_FLOAT(PACKAGENAME *bm,ADDRESS addr, float value){pid_t pid=getPID(bm);if (pid == 0){puts("can not get pid");return 0;}char lj[64];int handle;sprintf(lj, "/proc/%d/mem", pid);handle=open(lj, O_RDWR);lseek(handle, 0, SEEK_SET);pwrite64(handle, &value, 4, addr);close(handle);return 0;}int isapkrunning(PACKAGENAME *bm){DIR *dir=NULL;struct dirent *ptr=NULL;FILE *fp=NULL;char filepath[50];char filetext[128];dir=opendir("/proc/");if (dir!=NULL){while ((ptr=readdir(dir))!=NULL){if ((strcmp(ptr->d_name, ".")==0)||(strcmp(ptr->d_name, "..")==0))continue;if (ptr->d_type!=DT_DIR)continue;sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);fp=fopen(filepath, "r");if (NULL!=fp){fgets(filetext,sizeof(filetext),fp);if (strcmp(filetext,bm)==0){closedir(dir);return 1;}fclose(fp);}}}closedir(dir);return 0;}

int main(int argc,char **argv)
{

PACKAGENAME *bm="com.tencent.tmgp.pubgmhd";
int ipid=getPID(bm);
char mname[]="libUE4.so";
long int libbase=get_module_base(ipid, mname);
long int w1=libbase+0X46D73E0;
WriteAddress_FLOAT(bm,w1,-1904987454002165247);
puts("开启成功");
}

