#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/uio.h>

// 支持多个架构
#if defined(__arm__)
int process_readv_syscall = 376;
int process_writev_syscall = 377;
#elif defined(__aarch64__)
int process_readv_syscall = 270;
int process_writev_syscall = 271;
#elif defined(__i386__)
int process_readv_syscall = 347;
int process_writev_syscall = 348;
#else
int process_readv_syscall = 310;
int process_writev_syscall = 311;
#endif

// 读写内存核心代码
ssize_t readv(pid_t __pid, void *address, void *buffer, size_t size)
{
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    return syscall(process_readv_syscall, __pid, local, 1, remote, 1, 0);
}

ssize_t writev(pid_t __pid, void *address, void *buffer, size_t size)
{
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    return syscall(process_writev_syscall, __pid, local, 1, remote, 1, 0);
}

// 一些定义
#define LEN sizeof(struct MAPS)
#define QWORDSIZE sizeof(QWORD)
#define DWORDSIZE sizeof(DWORD)
#define WORDSIZE sizeof(WORD)
#define BYTESIZE sizeof(BYTE)
#define FLOATSIZE sizeof(FLOAT)
#define DOUBLESIZE sizeof(DOUBLE)
#define FRE sizeof(struct FREEZE)
typedef struct MAPS *PMAPS;     //存储maps的链表
typedef struct FREEZE *PFREEZE; //存储冻结的数据的链表
typedef int TYPE;               // 类型
typedef int RANGE;              // 内存范围
typedef int COUNT;              // 结果计算
typedef long int OFFSET;        // 偏移
typedef long int ADDRESS;       // 地址
typedef char PACKAGENAME;       // 包名

// 数据结构
typedef int64_t QWORD;
typedef int32_t DWORD;
typedef int16_t WORD;
typedef int8_t BYTE;
typedef float FLOAT;
typedef double DOUBLE;

struct MAPS
{
    long int addr;
    long int taddr;
    struct MAPS *next;
};

enum type
{
    TYPE_DWORD,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_WORD,
    TYPE_BYTE,
    TYPE_QWORD,
};

enum Range
{
    ALL,         //所有内存
    B_BAD,       //B内存
    C_ALLOC,     //Ca内存
    C_BSS,       //Cb内存
    C_DATA,      //Cd内存
    CODE_APP,    //Xa内存
    C_HEAP,      //Ch内存
    JAVA_HEAP,   //Jh内存
    A_ANONMYOUS, //A内存
    CODE_SYSTEM, //Xs内存
    STACK,       //S内存
    ASHMEM       //As内存
};

//
int getPID(PACKAGENAME *PackageName);
void initMemoryTools(char *b);
int SetSearchRange(TYPE type);
void ClearResults(); // 清空Res结果
void PrintResults(uintptr_t *res,int num); //打印Res里面的内容
void MemorySearch(char *value, TYPE type);
void MemoryOffset(char *value, long int offset, int type);          //搜索偏移
void RangeMemorySearch(char *from_value, char *to_value, int type); //范围搜索
void MemoryWrite(char *value, long int offset, int type);           //内存写入
long GetModuleBaseAddr(const char *module_name);                    //模块基址查询

int PrintResCount();

// 全局变量
int MemorySearchRange = 0; //0为所有
PMAPS Res = NULL;          //全局buff(保存数据的地方)
int ResCount = 0;          //结果数量
pid_t pid;                 // 进程ID
int gs = 0;                // 搜索计数



int PrintResCount()
{
return ResCount;
}


void resadv(uintptr_t address, void *buffer, size_t size){

    
readv(pid,(void*)address,buffer,size);

}



// 常用方法封装
int getPID(PACKAGENAME *PackageName)
{
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    FILE *fp = NULL;
    char filepath[256];     // 大小随意，能装下cmdline文件的路径即可
    char filetext[128];     // 大小随意，能装下要识别的命令行文本即可
    dir = opendir("/proc"); // 打开路径
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) // 循环读取路径下的每一个文件/文件夹
        {
            // 如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (ptr->d_type != DT_DIR)
                continue;
            sprintf(filepath, "/proc/%s/cmdline", ptr->d_name); // 生成要读取的文件的路径
            fp = fopen(filepath, "r");                          // 打开文件
            if (NULL != fp)
            {
                fgets(filetext, sizeof(filetext), fp); // 读取文件
                if (strcmp(filetext, PackageName) == 0)
                {
                    //puts(filepath);
                    //printf("packagename:%s\n",filetext);
                    break;
                }
                fclose(fp);
            }
        }
    }
    if (readdir(dir) == NULL)
    {
        //puts("Get pid fail");
        return 0;
    }
    closedir(dir); // 关闭路径
    return atoi(ptr->d_name);
}

void initMemoryTools(char *b)
{

    pid = getPID(b);
    if (pid == 0)
    {
        printf("\033[31;1m");
        puts("获取进程PID失败!");
        exit(1);
    }
}

int SetSearchRange(TYPE type)
{
    switch (type)
    {
    case ALL:
        MemorySearchRange = 0;
        break;
    case B_BAD:
        MemorySearchRange = 1;
        break;
    case C_ALLOC:
        MemorySearchRange = 2;
        break;
    case C_BSS:
        MemorySearchRange = 3;
        break;
    case C_DATA:
        MemorySearchRange = 4;
        break;
    case CODE_APP:
        MemorySearchRange = 5;
        break;
    case C_HEAP:
        MemorySearchRange = 6;
        break;
    case JAVA_HEAP:
        MemorySearchRange = 7;
        break;
    case A_ANONMYOUS:
        MemorySearchRange = 8;
        break;
    case CODE_SYSTEM:
        MemorySearchRange = 9;
        break;
    case STACK:
        MemorySearchRange = 10;
        break;
    case ASHMEM:
        MemorySearchRange = 11;
        break;
    default:
        printf("\033[32;1mYou Select A NULL Type!\n");
        break;
    }
    return 0;
}

void ClearResults() //清空结果
{
    PMAPS pHead = Res;
    PMAPS pTemp = pHead;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        pTemp = pHead;
        pHead = pHead->next;
        free(pTemp);
    }
}

void PrintResults(uintptr_t *res,int num)
{
    PMAPS temp = Res;
    const char *type = (char *)malloc(sizeof(char));
    for (int i = 0; i < num; i++)
    {
       res[i] =temp->addr;
        printf("addr:%lx\n", temp->addr);
        temp = temp->next; //指向下一个节点
    }
    
    

    
}

PMAPS readmaps()
{
    PMAPS pHead = NULL;
    PMAPS pNew;
    PMAPS pEnd;
    pEnd = pNew = (PMAPS)malloc(LEN);
    FILE *fp;
    int i = 0, flag = 1;
    char lj[64], buff[256];
    sprintf(lj, "/proc/%d/maps", pid);
    fp = fopen(lj, "r");
    if (fp == NULL)
    {
        puts("分析失败");
        exit(1);
    }
    while (!feof(fp))
    {
        fgets(buff, sizeof(buff), fp); //读取一行
        switch (MemorySearchRange)
        {
        case ALL:
            if (strstr(buff, "rw") != NULL && !feof(fp))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case B_BAD:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "kgsl-3d0"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case C_ALLOC:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[anon:libc_malloc]"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case C_BSS:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[anon:.bss]"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case C_DATA:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/data/app/") || strstr(buff, "/data/data/") && strstr(buff, ".so"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case CODE_APP:
            //if (strstr(buff, "r-xp") != NULL && !feof(fp) && strstr(buff, "/data/app/") || strstr(buff, "/data/data/") && !strstr(buff, "dalvik"))
            if (!feof(fp) && strstr(buff, "/data/app/") || strstr(buff, "/data/data/"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
        case C_HEAP:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[heap]"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
        case JAVA_HEAP:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/dev/ashmem/"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case A_ANONMYOUS:
            //if (strstr(buff, "rw") != NULL && !feof(fp) && (strlen(buff) < 42))
            if (strstr(buff, "rw-p") != NULL && !feof(fp) && (strlen(buff) < 42))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case CODE_SYSTEM:
            //if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/system/") || strstr(buff, "/vendor/") || strstr(buff, "/dev/ashmem/"))
            if (!feof(fp) && strstr(buff, "/system/") || strstr(buff, "/vendor/") || strstr(buff, "/dev/ashmem/"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case STACK:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "[stack"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        case ASHMEM:
            if (strstr(buff, "rw") != NULL && !feof(fp) && strstr(buff, "/dev/ashmem/") && !strstr(buff, "dalvik"))
            {
                sscanf(buff, "%lx-%lx", &pNew->addr, &pNew->taddr);
                //这里使用lx是为了能成功读取特别长的地址
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            break;
        default:
            printf("\033[32;1mYou Select A NULL Type!\n");
            break;
        }
        if (flag == 1)
        {
            i++;
            if (i == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                pHead = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN); //分配内存
        }
    }
    free(pNew); //将多余的空间释放
    fclose(fp); //关闭文件指针
    return pHead;
}

PMAPS MemorySearch_DWORD(DWORD value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / DWORDSIZE + 1;
        DWORD *buff = (DWORD *)malloc(c * DWORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + DWORDSIZE * i;
                //n->type = TYPE_DWORD;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS MemorySearch_FLOAT(FLOAT value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / FLOATSIZE + 1;
        FLOAT *buff = (FLOAT *)malloc(c * FLOATSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + FLOATSIZE * i;
                //n->type = TYPE_FLOAT;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS MemorySearch_DOUBLE(DOUBLE value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / DOUBLESIZE + 1;
        DOUBLE *buff = (DOUBLE *)malloc(c * DOUBLESIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + DOUBLESIZE * i;
                // n->type = TYPE_DOUBLE;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS MemorySearch_WORD(WORD value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / WORDSIZE + 1;
        WORD *buff = (WORD *)malloc(c * WORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + WORDSIZE * i;
                //n->type = TYPE_WORD;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS MemorySearch_BYTE(BYTE value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / BYTESIZE + 1;
        BYTE *buff = (BYTE *)malloc(c * BYTESIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + BYTESIZE * i;
                //n->type = TYPE_BYTE;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS MemorySearch_QWORD(QWORD value, PMAPS pMap)
{
    if (pid == 0)
    {
        puts("can not get pid");
        return NULL;
    }
    gs = 0;
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL) //读取maps里面的地址
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / QWORDSIZE + 1;
        QWORD *buff = (QWORD *)malloc(c * QWORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        for (int i = 0; i < c; i++)
        {
            if (value == buff[i])
            {
                iCount++;
                gs += 1;
                n->addr = (pTemp->addr) + QWORDSIZE * i;
                //n->type = TYPE_QWORD;
                //printf("搜索结果：addr:%lx,buff=%d\n", n->addr, buff[i]);
                if (iCount == 1)
                {
                    n->next = NULL;
                    e = n;
                    pBuff = n;
                }
                else
                {
                    n->next = NULL;
                    e->next = n;
                    e = n;
                }
                n = (PMAPS)malloc(LEN);
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

void MemorySearch(char *value, TYPE type)
{
    PMAPS pMap = NULL;
    PMAPS pHead = NULL;
    pMap = readmaps();
    if (pMap == NULL)
    {
        puts("readmap error");
        exit(1);
    }
    switch (type)
    {
    case TYPE_DWORD:
        pHead = MemorySearch_DWORD(atoi(value), pMap);
        break;
    case TYPE_FLOAT:
        pHead = MemorySearch_FLOAT(atof(value), pMap);
        break;
    case TYPE_DOUBLE:
        pHead = MemorySearch_DOUBLE(atof(value), pMap);
        break;
    case TYPE_WORD:
        pHead = MemorySearch_WORD(atoi(value), pMap);
        break;
    case TYPE_BYTE:
        pHead = MemorySearch_BYTE(atoi(value), pMap);
        break;
    case TYPE_QWORD:
        pHead = MemorySearch_QWORD(atoll(value), pMap);
        break;
    }
    ResCount = gs;
    Res = pHead; //Res指针指向链表
}

PMAPS MemoryOffset_DWORD(DWORD value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    DWORD *buf = (DWORD *)malloc(DWORDSIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, DWORDSIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_DWORD;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}

PMAPS MemoryOffset_FLOAT(FLOAT value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    FLOAT *buf = (FLOAT *)malloc(FLOATSIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, FLOATSIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_FLOAT;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}

PMAPS MemoryOffset_DOUBLE(DOUBLE value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    DOUBLE *buf = (DOUBLE *)malloc(DOUBLESIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, DOUBLESIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_DOUBLE;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}
PMAPS MemoryOffset_WORD(WORD value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    WORD *buf = (WORD *)malloc(WORDSIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, WORDSIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_WORD;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}
PMAPS MemoryOffset_BYTE(BYTE value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    BYTE *buf = (BYTE *)malloc(BYTESIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, BYTESIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_BYTE;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}
PMAPS MemoryOffset_QWORD(QWORD value, OFFSET offset, PMAPS pBuff)
{
    PMAPS pEnd = NULL;
    PMAPS pNew = NULL;
    PMAPS pTemp = pBuff;
    PMAPS BUFF = NULL;
    pEnd = pNew = (PMAPS)malloc(LEN);
    BUFF = pNew;
    int iCount = 0;
    long int naddr;
    QWORD *buf = (QWORD *)malloc(QWORDSIZE);
    while (pTemp != NULL)
    {
        naddr = pTemp->addr + offset;
        readv(pid, reinterpret_cast<void *>(naddr), buf, QWORDSIZE);
        if (value == *buf)
        {
            //printf("偏移结果：addr:%lx,buff=%d\n", naddr, *buf);
            iCount++;
            gs += 1;
            pNew->addr = pTemp->addr;
            //pNew->type = TYPE_QWORD;
            if (iCount == 1)
            {
                pNew->next = NULL;
                pEnd = pNew;
                BUFF = pNew;
            }
            else
            {
                pNew->next = NULL;
                pEnd->next = pNew;
                pEnd = pNew;
            }
            pNew = (PMAPS)malloc(LEN);
            if (ResCount == 1)
            {
                free(pNew);
                return BUFF;
            }
        }
        pTemp = pTemp->next; //指向下一个节点读取数据
    }
    free(pNew);
    return BUFF;
}

void MemoryOffset(char *value, OFFSET offset, TYPE type)
{
    PMAPS pHead = NULL;
    gs = 0; //临时全局计数
    switch (type)
    {
    case TYPE_DWORD:
        pHead = MemoryOffset_DWORD(atoi(value), offset, Res);
        break;
    case TYPE_FLOAT:
        pHead = MemoryOffset_FLOAT(atof(value), offset, Res);
        break;
    case TYPE_DOUBLE:
        pHead = MemoryOffset_DOUBLE(atof(value), offset, Res);
        break;
    case TYPE_WORD:
        pHead = MemoryOffset_WORD(atoi(value), offset, Res);
        break;
    case TYPE_BYTE:
        pHead = MemoryOffset_BYTE(atoi(value), offset, Res);
        break;
    case TYPE_QWORD:
        pHead = MemoryOffset_QWORD(atoll(value), offset, Res);
        break;
    default:
        printf("\033[32;1mYou Select A NULL Type!\n");
        break;
    }
    if (pHead == NULL)
    {
        puts("偏移失败!");
        exit(1);
    }
    ResCount = gs;  //全局个数
    ClearResults(); //清空存储的数据(释放空间)
    Res = pHead;    //指向新搜索到的空间
}

///
PMAPS RangeMemorySearch_DWORD(DWORD from_value, DWORD to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / DWORDSIZE + 1;
        DWORD *buff = (DWORD *)malloc(c * DWORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + DWORDSIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS RangeMemorySearch_FLOAT(FLOAT from_value, FLOAT to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / FLOATSIZE + 1;
        FLOAT *buff = (FLOAT *)malloc(c * FLOATSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + FLOATSIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS RangeMemorySearch_DOUBLE(DOUBLE from_value, DOUBLE to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / DOUBLESIZE + 1;
        DOUBLE *buff = (DOUBLE *)malloc(c * DOUBLESIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + DOUBLESIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS RangeMemorySearch_WORD(WORD from_value, WORD to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / WORDSIZE + 1;
        WORD *buff = (WORD *)malloc(c * WORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + WORDSIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS RangeMemorySearch_BYTE(BYTE from_value, BYTE to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / BYTESIZE + 1;
        BYTE *buff = (BYTE *)malloc(c * BYTESIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + BYTESIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

PMAPS RangeMemorySearch_QWORD(QWORD from_value, QWORD to_value, PMAPS pMap)
{
    PMAPS pTemp = NULL;
    pTemp = pMap;
    PMAPS n, e;
    e = n = (PMAPS)malloc(LEN);
    PMAPS pBuff;
    pBuff = n;
    int iCount = 0; //链表长度
    int c;
    int64_t iSize;
    while (pTemp != NULL)
    {
        iSize = pTemp->taddr - pTemp->addr;
        c = iSize / QWORDSIZE + 1;
        QWORD *buff = (QWORD *)malloc(c * QWORDSIZE);
        readv(pid, reinterpret_cast<void *>(pTemp->addr), buff, iSize);
        {
            for (int i = 0; i < c; i++)
            {
                if (buff[i] >= from_value && buff[i] <= to_value)
                { //判断值是否在这两者之间
                    iCount++;
                    gs += 1;
                    n->addr = (pTemp->addr) + QWORDSIZE * i;
                    if (iCount == 1)
                    {
                        n->next = NULL;
                        e = n;
                        pBuff = n;
                    }
                    else
                    {
                        n->next = NULL;
                        e->next = n;
                        e = n;
                    }
                    n = (PMAPS)malloc(LEN);
                }
            }
        }
        pTemp = pTemp->next;
        free(buff);
    }
    free(n);
    return pBuff;
}

void RangeMemorySearch(char *from_value, char *to_value, TYPE type)
{
    PMAPS pHead = NULL;
    PMAPS pMap = NULL;
    gs = 0;
    pMap = readmaps();
    if (pMap == NULL)
    {
        puts("readmap error");
        exit(1);
    }
    switch (type)
    {
    case TYPE_DWORD:
        if (atoi(from_value) > atoi(to_value))
            pHead = RangeMemorySearch_DWORD(atoi(to_value), atoi(from_value), pMap);
        else
            pHead = RangeMemorySearch_DWORD(atoi(from_value), atoi(to_value), pMap);
        break;
    case TYPE_FLOAT:
        if (atof(from_value) > atof(to_value))
            pHead = RangeMemorySearch_FLOAT(atof(to_value), atof(from_value), pMap);
        else
            pHead = RangeMemorySearch_FLOAT(atof(from_value), atof(to_value), pMap);
        break;
    case TYPE_DOUBLE:
        if (atof(from_value) > atof(to_value))
            pHead = RangeMemorySearch_DOUBLE(atof(to_value), atof(from_value), pMap);
        else
            pHead = RangeMemorySearch_DOUBLE(atof(from_value), atof(to_value), pMap);
        break;
    case TYPE_WORD:
        if (atoi(from_value) > atoi(to_value))
            pHead = RangeMemorySearch_WORD(atoi(to_value), atoi(from_value), pMap);
        else
            pHead = RangeMemorySearch_WORD(atoi(from_value), atoi(to_value), pMap);
        break;
    case TYPE_BYTE:
        if (atoi(from_value) > atoi(to_value))
            pHead = RangeMemorySearch_BYTE(atoi(to_value), atoi(from_value), pMap);
        else
            pHead = RangeMemorySearch_BYTE(atoi(from_value), atoi(to_value), pMap);
        break;
    case TYPE_QWORD:
        if (atoll(from_value) > atoll(to_value))
            pHead = RangeMemorySearch_QWORD(atoll(to_value), atoll(from_value), pMap);
        else
            pHead = RangeMemorySearch_QWORD(atoll(from_value), atoll(to_value), pMap);
        break;
    default:
        printf("\033[32;1mYou Select A NULL Type!\n");
        break;
    }
    if (pHead == NULL)
    {
        puts("Range搜索内存失败!");
        exit(1);
    }
    ResCount = gs;
    Res = pHead; //Res指针指向链表
}

int MemoryWrite_DWORD(DWORD value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(DWORD));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

int MemoryWrite_FLOAT(FLOAT value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(FLOAT));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

int MemoryWrite_DOUBLE(DOUBLE value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    int handle;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(DOUBLE));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

int MemoryWrite_WORD(WORD value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(WORD));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

int MemoryWrite_BYTE(BYTE value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(BYTE));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

int MemoryWrite_QWORD(QWORD value, PMAPS pBuff, OFFSET offset)
{

    PMAPS pTemp = NULL;
    pTemp = pBuff;
    int i;
    for (i = 0; i < ResCount; i++)
    {
        writev(pid, reinterpret_cast<void *>(pTemp->addr + offset), &value, sizeof(QWORD));
        if (pTemp->next != NULL)
            pTemp = pTemp->next;
    }
    return 0;
}

void MemoryWrite(char *value, OFFSET offset, TYPE type)
{
    switch (type)
    {
    case TYPE_DWORD:
        MemoryWrite_DWORD(atoi(value), Res, offset);
        break;
    case TYPE_FLOAT:
        MemoryWrite_FLOAT(atof(value), Res, offset);
        break;
    case TYPE_DOUBLE:
        MemoryWrite_DOUBLE(atof(value), Res, offset);
        break;
    case TYPE_WORD:
        MemoryWrite_WORD(atoi(value), Res, offset);
        break;
    case TYPE_BYTE:
        MemoryWrite_BYTE(atoi(value), Res, offset);
        break;
    case TYPE_QWORD:
        MemoryWrite_QWORD(atoll(value), Res, offset);
        break;
    default:
        printf("\033[32;1mYou Select A NULL Type!\n");
        break;
    }
    //ClearResults();//清空list
}

long GetModuleBaseAddr(const char *module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    if (pid < 0)
    {
        /* self process */
        snprintf(filename, sizeof(filename), "/proc/self/maps", pid);
    }
    else
    {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}
