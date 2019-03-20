#include <android/log.h>
#include "utils.h"
//#include "ida.h"
#include <string.h>
#include <stdio.h>
#include <hookzz.h>
#include <../include/hookzz.h>
#include <sys/system_properties.h>
#include <jni.h>
#include <time.h>
#include <stdlib.h>

#define LOG_TAG "Xposed:native:log"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__);
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);

#define REG(X) (rs->general.regs.r##X)
#define reg0 REG(0)
#define reg1 REG(1)
#define reg2 REG(2)
#define reg3 REG(3)

#define BUF_LEN 81920
uint32_t lib_addr;
unsigned char buf[BUF_LEN] = {'\0'};
unsigned char DIVISION[8] = {'\n','\n','\0','\0','\0','\0','\n','\n'};

typedef struct _item{
    uint32_t addr;
    PRECALL pre_call_ptr;
    POSTCALL post_call_ptr;
    uint32_t isThumb;   // 0 for false or 1 for true
} Item;



void pre_send(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_send ---------------");
//	bt();
}

void aft_send(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_send ---------------");
//	bt();
}


void hookDHXY(){
	const char *libName = "libG17.so";
	const char *procName = "com.netease.dhxy";

	LOGD("init");
	lib_addr = get_lib_base(procName,libName);
	LOGD("libaddr:%x", lib_addr);


    if( (int)lib_addr != 0 && (int)lib_addr != -1 ){
        LOGD("hooked lib:%s, proc:%s, base addr:%p", libName, procName, lib_addr);
        Item funcItems[] = {
                { 0x007E683C, pre_send, aft_send, false},
        };

        for(int i = 0; i < sizeof(funcItems)/sizeof(funcItems[0]) ; i ++){
            zpointer tgt = lib_addr + funcItems[i].addr + ( funcItems[i].isThumb ? 1 : 0 );
            ZzBuildHook(tgt, NULL, NULL, funcItems[i].pre_call_ptr, funcItems[i].post_call_ptr, false);
            ZzEnableHook(tgt);
        }
    }
}

__attribute__((constructor)) void hook(){
	hookDHXY();
}
