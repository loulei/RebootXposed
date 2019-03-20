//
// Created by mbx on 2017/12/12.
//

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <android/log.h>

#define LOG_TAG "Xposed:Inject:so"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__);


#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>


static void *gHandle = NULL;

void getPid(){
    LOGD("current tid=%lu",pthread_self());
}

int find_pid_of(const char *process_name) {
    int id;
    pid_t pid = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];

    struct dirent *entry;

    if (process_name == NULL)
        return -1;

    dir = opendir("/proc");
    if (dir == NULL)
        return -1;

    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);

                if (strcmp(process_name, cmdline) == 0) {
                    /* process found */
                    pid = id;
                    break;
                }
            }
        }
    }

    closedir(dir);
    return pid;
}

uint32_t get_lib_base(const char* proc_name,const char* lib_name){
    pid_t pid = find_pid_of(proc_name);
    if (-1 == pid) {
        printf("Can't find the process\n");
        return -1;
    }
    FILE *fp;
    uint32_t addr = 0;
    char *pch;
    char filename[32];
    char line[1024];

    if (pid < 0) {
        /* self process */
        snprintf(filename, sizeof(filename), "/proc/self/maps", pid);
    } else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }

    fp = fopen(filename, "r");

    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, lib_name)) {
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

//字节流转换为十六进制字符串
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{

    static const char *c = "0123456789ABCDEF";
    int i = 0;
    for ( ; i < sourceLen; i++)
    {
        dest[i * 2] = c [source[i] >> 4];
        dest[i * 2 + 1] = c [source[i] & 0x0f];
//        dest[i * 3 + 2] = ' ';
    }
    dest[i * 2 + 2] = '\0';
    return ;
}


void ByteAdapterToStr(const char* source, char* dest, int sourceLen)
{

    static const char *c = "0123456789ABCDEF";
    int i = 0;
    int j = 0;
    for (; i < sourceLen; i++)
    {
        if( source[i] < 0x80 && source[i] >= 0x20){
            dest[j ] = source[i];
            dest[++j] = ' ';
        } else {
            dest[j] = '0';
            dest[ ++j ] = 'x';
            dest[++j] = c [source[i] >> 4];
            dest[++j] = c [source[i] & 0x0f];
            dest[++j] = ' ';
        }
        j ++;
    }
    dest[i * 3 + 3] = '\0';
    return ;
}

//十六进制字符串转换为字节流
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{

}