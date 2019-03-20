//
// Created by mbx on 2017/11/20.
//
#include <HookZz/include/hookzz.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <android/log.h>

#include "utils.h"

#define LOG_TAG "Xposed:Inject"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__);


const char *libName = "libtnet-3.1.10.so";
const char *procName = "com.autonavi.minimap";


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


void *get_lib_base(const char *procName, const char *lib_name) {
    pid_t pid = find_pid_of(procName);
    if (-1 == pid) {
        printf("Can't find the process\n");
        return -1;
    }
    FILE *fp;
    long addr = 0;
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
    return (void *) addr;
}

typedef int _DWORD;
typedef int16_t _WORD;
typedef int8_t _BYTE;



void printStr(const unsigned char *str,int len){
    char cache[512] = {'\0'};
    if (len < 512 / 3 - 1) {
        ByteToHexStr(str, cache, len);
        LOGD("hooked so data:%s", cache);
    } else {
        LOGD("hooked so data too large:%d", len);
        ByteToHexStr(str, cache, 512/3-1);
        LOGD("hooked so data:%s", cache);
    }
}

void pre_call246dc(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call ---------0x246dc");

    char cache[512] = {'\0'};

    if ((long) rs->general.regs.r2 < 512 / 3 - 1) {
        ByteToHexStr((unsigned char *) rs->general.regs.r1, cache, (long) rs->general.regs.r2);
        LOGD("hooked so pre call r1:%s", cache);
    } else {
        LOGD("hooked so pre call r1 too large:%d", (long) rs->general.regs.r2);
        ByteToHexStr((unsigned char *) rs->general.regs.r1, cache, 150);
        LOGD("hooked so pre call r1:%s", cache);
    }


}

void pre_call24170(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0x24170");

    int a1 = (int)rs->general.regs.r0 ;

    char *a2 = (char*) rs->general.regs.r1;
    int a3 = (int)rs->general.regs.r2;

    char * str =  (char *) ( *(_DWORD *)(a1 + 28) + *(_DWORD *)(a1 + 16) );
    printStr( str,a3);

}


void pre_call24520(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0x24520");

    int a1 = (int)rs->general.regs.r0 ;
    char *a2 = (char*) rs->general.regs.r1;
    int a3 = (int)rs->general.regs.r2;


    char * str =  (char *) ( *(_DWORD *)(a1 + 28) + *(_DWORD *)(a1 + 16) );
    printStr( str,a3);

}
void pre_call1193c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0x1193c");

    int a1 = (int)rs->general.regs.r0 ;
    char *a2 = (char*) rs->general.regs.r1;
    int a3 = (int)rs->general.regs.r2;

    int v7 = *(_DWORD *)(a1 + 348);

    char * str =  (char *) ( *(_DWORD *)(v7 + 28) + *(_DWORD *)(v7 + 16) );
    printStr( str,10);

}

void pre_call23ed8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------23ed8");
    int a1 = (int)rs->general.regs.r0 ;
    char *a2 = (char*) rs->general.regs.r1;
    int a3 = (int)rs->general.regs.r2;


    char * str =  (char *) ( *(_DWORD *)(a1 + 28) + *(_DWORD *)(a1 + 16) );
    printStr( str,a3);
}



void pre_callcc5c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0xcc5c");

    int a1 = (int)rs->general.regs.r0 ;

    uint32_t lr = rs->lr;
    int v7 = *(_DWORD *)(a1 + 348);

    char * str =  (char *) ( *(_DWORD *)(v7 + 28) + *(_DWORD *)(v7 + 16) );
    printStr( str,20);
    //LOGD("hooked so LR:%ud",lr);
}

void pre_calleb6c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0xeb6c");

    int a1 = (int)rs->general.regs.r0 ;
    int v7 = *(_DWORD *)(a1 + 348);
    char * str =  (char *) ( *(_DWORD *)(v7 + 28) + *(_DWORD *)(v7 + 16) );
    printStr( str,20);

}

void pre_calla630(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------0xa630");
    char *v10 = (char*)rs->general.regs.r1;
    STACK_SET(callstack, "v10", v10 , char *);
    LOGD("hooked so a1:%p,a2:%p",rs->general.regs.r0,rs->general.regs.r1);
/*
    int v5 =  *(_DWORD *)v10 - 12;
    int v7 = *(_DWORD *)(v5 + 348) ;
    char * str =  (char *) ( *(_DWORD *)(v7 + 28) + *(_DWORD *)(v7 + 16) );
    printStr( str,10);

 */
}
void after_calla630(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------0xa630");

    if (STACK_CHECK_KEY(callstack, "v10")) {
        char *v10 = STACK_GET(callstack, "v10", char *);
        int v5 =  *(_DWORD *)v10 - 12;
        int v7 = *(_DWORD *)(v5 + 348) ;
        char * str =  (char *) ( *(_DWORD *)(v7 + 28) + *(_DWORD *)(v7 + 16) );
        printStr( str,10);
    }
}

void pre_call1a9b4(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_call 1a9b4 encode");

}
void after_call1a9b4(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------after_call 1a9b4 encode");
    LOGD("hooked so ret:%d",rs->general.regs.r0);
}

void pre_call19304(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------19304");

    char *a2 = rs->general.regs.r2;

    LOGD("hooked so:%s",a2);
}

void pre_call12a08(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
        LOGD("hooked so after call -----------12a08");

}

void pre_call1a248(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------1a248");
    int a3 = rs->general.regs.r2;
    int **a2 = rs->general.regs.r1;
    int a1 = rs->general.regs.r0;

    int v23 = a1;
    int v22 = a3;
    int *v24 = *a2;
    int v25 = **a2;
    int *v3 = a2[15];

    int v6 =  *(_DWORD *)(v22 + 28);

    LOGD("hooked so path:%s,v6:%p,v24:%p", *(void **)(v22 + 12),v6,v24);
}


void after_call1a248(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------1a248");

    LOGD("hooked so ret:%d", rs->general.regs.r0);
}
void pre_call1a1bc(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------1a1bc");

}
void after_call1a1bc(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------1a1bc");
    LOGD("hooked so ret:%p",rs->general.regs.r0);
}
void after_call1b758(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------1b758");

}
void pre_call1a828(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------1a828");
}


///////////////--------------------------------------------
void pre_0x5368(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x5368");
}
void pre_0x5c84(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x5c84");
    LOGD("hooked so LR:%p",rs->lr);
}
void pre_0x5db8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x5db8");
}
void pre_0x5ed4(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x5ed4");
}
void pre_0x6210(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x6210");
    LOGD("hooked so LR:%p",rs->lr);
}
void pre_0x6314(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x6314");
}
void pre_0x6828(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x6828");
}
void pre_0x6c18(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x6c18");
}
void pre_0x6f68(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x6f68");
    LOGD("hooked so LR:%p",rs->lr);
}
void pre_0x7488(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x7488");
}
void pre_0x8a94(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x8a94");
}
void pre_0x9348(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x9348");
}
void pre_0x9e2c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x9e2c");
    void *a1 = rs->general.regs.r0;
    STACK_SET(callstack, "a1", a1 , void *);
    LOGD("hooked so a1:%s",a1);
    uint32_t a2 = rs->general.regs.r1;
    uint32_t a3 = rs->general.regs.r2;
    LOGD("hooked so a2:%ud,a3:%ud,a4:%d",a2,a3,rs->general.regs.r3);
}

void after_0x9e2c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so after call -----------after_0x9e2c");
    void *a1;
    if (STACK_CHECK_KEY(callstack, "a1")) {
        a1 = STACK_GET(callstack, "a1", void *);
    }
    LOGD("hooked so r1:%s,ret:%s",a1,rs->general.regs.r0);
}

void pre_0x9ffc(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x9ffc");
}
void pre_0xe790(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0xe790");
}
void pre_0xeca8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0xeca8");
}
void pre_0x10f7c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x10f7c");
}
void pre_0x12830(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x12830");
}
void pre_0x153f0(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x153f0");
}
void pre_0x156e4(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x156e4");
}
void pre_0x13e0c(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x13e0c");
}
void pre_0x14ea8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x14ea8");
}
void pre_0x15330(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x15330");
}
void pre_0x15bf8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x15bf8");
}
void pre_0x15ea8(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x15ea8");
}
void pre_0x161bb(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x161bb");
}
void pre_0x16424(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x16424");
}
void pre_0x18b98(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x18b98");
}
void pre_0x19304(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x19304");
}
void pre_0x1cdd0(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x1cdd0");
}
void pre_0x1ee48(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x1ee48");
}
void pre_0x21e10(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x21e10");
}
void pre_0x24170(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x24170");
}
void pre_0x34f08(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_0x34f08");
}




//--------------------------------------------------------------------------------
__attribute__ (( constructor )) void hook() {

    zpointer lib_addr = (uint32_t) get_lib_base(procName, libName);
    LOGD("hooked lib:%s, base addr:%p", libName, lib_addr);


    /*
    zpointer tgt_addr2635c = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x2635c;
    ZzBuildHook(tgt_addr2635c, NULL, NULL,set_stack_pre_call, after_cal12635c,  false);
    ZzEnableHook(tgt_addr2635c);
    LOGD("hooked so addr:%p", tgt_addr2635c);

    zpointer tgt_addr25ac4 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x25ac4;
    ZzBuildHook(tgt_addr25ac4, NULL, NULL,pre_call25ac4, after_cal125ac4,  false);
    ZzEnableHook(tgt_addr25ac4);
    LOGD("hooked so addr:%p", tgt_addr25ac4);
*/
    /*
    zpointer tgt_addr20604 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x20604;
    ZzBuildHook(tgt_addr20604, NULL, NULL, pre_cal120604, NULL, false);
    ZzEnableHook(tgt_addr20604);
    LOGD("hooked so addr:%p", tgt_addr20604);

    zpointer tgt_addr153f0 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x153f0;
    ZzBuildHook(tgt_addr153f0, NULL, NULL, pre_cal153f0, NULL, false);
    ZzEnableHook(tgt_addr153f0);
    LOGD("hooked so addr:%p", tgt_addr153f0);

    zpointer tgt_addr18948 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x18948;
    ZzBuildHook(tgt_addr18948, NULL, NULL, pre_cal18948, NULL, false);
    ZzEnableHook(tgt_addr18948);
    LOGD("hooked so addr:%p", tgt_addr18948);


    //0x26374

    //0x25768
    //0x25468

    //246DC

    zpointer tgt_addr = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x25768;
    ZzBuildHook(tgt_addr, NULL, NULL, pre_call, NULL, false);
    ZzEnableHook(tgt_addr);
    LOGD("hooked so addr:%p", tgt_addr);


    zpointer tgt_addr1 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x25468;
    ZzBuildHook(tgt_addr1, NULL, NULL, pre_call25468, NULL, false);
    ZzEnableHook(tgt_addr1);
    LOGD("hooked so addr:%p", tgt_addr1);


    zpointer tgt_addr2 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x246DC;
    ZzBuildHook(tgt_addr2, NULL, NULL, pre_call246dc, NULL, false);
    ZzEnableHook(tgt_addr2);
    LOGD("hooked so addr:%p", tgt_addr2);

*/


/*
    zpointer tgt_addr4 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x4178;
    ZzBuildHook(tgt_addr4, NULL, NULL, pre_call4178, NULL, false);
    ZzEnableHook(tgt_addr4);
    LOGD("hooked so addr:%p", tgt_addr4);

    zpointer tgt_addr5 = (uint32_t) lib_addr + (uint32_t) 1 + (uint32_t) 0x423c;
    ZzBuildHook(tgt_addr5, NULL, NULL, pre_call423c, NULL, false);
    ZzEnableHook(tgt_addr5);
    LOGD("hooked so addr:%p", tgt_addr5);
*/
}


/**
__attribute__((constructor)) void hook(){
    zpointer lib_addr = (uint32_t) get_lib_base(procName,libName) ;
    LOGD("hooked libnative-lib.so,lib addr:%p",lib_addr)
    zpointer tgt_addr = (uint32_t) lib_addr + (uint32_t) 0x600;
    LOGD("hooked libnative-lib.so:%p",tgt_addr);


    //ZzBuildHookAddress(tgt_addr,tgt_addr + 2,NULL,NULL,false);
    //ZzEnableHook(tgt_addr);

    ZzEnableDebugMode();
    //ZzHookAddress(tgt_addr , tgt_addr + 4, NULL, NULL);

    // 小端
    //unsigned long nop_bytes = 0x00002010;   // mov r0,10
    //ZzRuntimeCodePatch((unsigned long) tgt_addr, (zpointer)&nop_bytes, 2);


    //zpointer code = 0x4620;
    //ZzRuntimeCodePatch((unsigned long) tgt_addr, code ,2);

}

*/
/*
__attribute__ (( constructor )) void hook(){
    int (*oriaddr)(int ,int )  = NULL;
    zpointer lib_addr = (uint32_t) get_lib_base(procName,libName) ;
    LOGD("hooked libnative-lib.so,lib addr:%p",lib_addr)
    zpointer tgt_addr = (uint32_t) lib_addr + (uint32_t) 0x683 ;
    LOGD("hooked libnative-lib.so:%p",tgt_addr);

    ZzEnableDebugMode();
    ZzBuildHookAddress(tgt_addr,tgt_addr + 2,pre_call,half_call,false);
    ZzEnableHook(tgt_addr);
}


*/