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

int array_len = 0;

void pre_GetMethodID(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
    LOGD("------------ pre_GetMethodID ---------------");
    LOGD("r2=%s", reg2);
    LOGD("r3=%s", reg3);
}

void aft_GetMethodID(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
    LOGD("------------ aft_GetMethodID ---------------");
    LOGD("methodId=0x%08x", reg0);
}

void pre_getSignFromJni(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	 LOGD("------------ pre_getSignFromJni ---------------");
}

void aft_getSignFromJni(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	 LOGD("------------ aft_getSignFromJni ---------------");
}

void pre_FindClass(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_FindClass ---------------");
	LOGD("r1=%s", reg1);
}

void aft_FindClass(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_FindClass ---------------");
}

void pre_NewStringUTF(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_NewStringUTF ---------------");
	LOGD("r1=%s", reg1);
}

void aft_NewStringUTF(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_NewStringUTF ---------------");
}

void pre_GetArrayLength(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_GetArrayLength ---------------");
}

void aft_GetArrayLength(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_GetArrayLength ---------------");
	int size = reg0;
	array_len = size;
	LOGD("return=%d", size);
}

void pre_NewObjectV(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_NewObjectV ---------------");
	LOGD("mid=0x%08x", reg2);
}

void aft_NewObjectV(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_NewObjectV ---------------");
	LOGD("jobject=0x%08x", reg0);
}

void pre_md4(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_md4 ---------------");
	unsigned char *v63 = reg0;
	int len = reg1;
	ByteToHexStr(reg1,buf,len);
	LOGD("pre_md4:%s, len=%d", buf, len);
}

void aft_md4(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_md4 ---------------");
}

void pre_GetByteArrayElements(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_GetByteArrayElements ---------------");
	jbyteArray array = reg1;
	JNIEnv *env = reg0;
	jsize len = (*env)->GetArrayLength(env, reg1);
	LOGD("pre_GetByteArrayElements: len=%d", len);
	STACK_SET(callstack, "len", len, uint32_t);
}

void aft_GetByteArrayElements(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_GetByteArrayElements ---------------");
	uint32_t len = STACK_GET(callstack, "len", uint32_t);
	LOGD("aft_GetByteArrayElements: len=%d", len);
//	memset(buf, 0, BUF_LEN);
	if(len){
		unsigned char *ret = reg0;
		unsigned char *buf = (unsigned char *)malloc(BUF_LEN);
		memset(buf, 0, BUF_LEN);
		LOGD("aft_GetByteArrayElements: r0=0x%08x, r1=0x%08x, r9=0x%08x", reg0, reg1, rs->general.regs.r9);
		ByteToHexStr(ret, buf, len);
		LOGD("aft_GetByteArrayElements: %s", buf);
		free(buf);
	}

}

void pre_GetByteArrayRegion(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_GetByteArrayRegion ---------------");
	STACK_SET(callstack, "sp", rs->sp, uint32_t);
}

void aft_GetByteArrayRegion(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ aft_GetByteArrayRegion ---------------");
//	unsigned char *str = rs->sp;
	uint32_t sp_addr = STACK_GET(callstack, "sp", uint32_t) + 0xF0;
	unsigned char *str = sp_addr;
	LOGD("aft_GetByteArrayRegion: %s", str);

}

void pre_usleep(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_usleep ---------------");
	int sec = reg0;
	LOGD("pre_usleep: sec=%d", sec);
}

void pre_strlen(RegState *rs, ThreadStack *threadstack, CallStack *callstack){
	LOGD("------------ pre_strlen ---------------");
	unsigned char *str = reg0;
	LOGD("pre_strlen: str=%s", str);
}

typedef struct _item{
    uint32_t addr;
    PRECALL pre_call_ptr;
    POSTCALL post_call_ptr;
    uint32_t isThumb;   // 0 for false or 1 for true
} Item;


void hookArt(){
    const char *libName = "libart.so";

    const char *procName = "com.jingdong.app.mall";

    LOGD("init");
    lib_addr = get_lib_base(procName,libName);
    LOGD("libaddr:%x", lib_addr);


    if( (int)lib_addr != 0 && (int)lib_addr != -1 ){
        LOGD("hooked lib:%s, proc:%s, base addr:%p", libName, procName, lib_addr);
        Item funcItems0[] = {
                { 0x001AE930, pre_GetMethodID, aft_GetMethodID, true},
				{ 0x001D1654, pre_FindClass, aft_FindClass, true},
				{ 0x001AB854, pre_NewStringUTF, aft_NewStringUTF, true},
				{ 0x001BC084, pre_GetByteArrayElements, aft_GetByteArrayElements, true},
//				{ 0x001BBBB4, pre_GetByteArrayRegion, aft_GetByteArrayRegion, true},
//				{ 0x001BD2C4, pre_GetArrayLength, aft_GetArrayLength, true},
//				{ 0x001D23A0, pre_NewObjectV, aft_NewObjectV, true},
        };

        for(int i = 0; i < sizeof(funcItems0)/sizeof(funcItems0[0]) ; i ++){
            zpointer tgt = lib_addr + funcItems0[i].addr + ( funcItems0[i].isThumb ? 1 : 0 );
            ZzBuildHook(tgt, NULL, NULL, funcItems0[i].pre_call_ptr, funcItems0[i].post_call_ptr, false);
            ZzEnableHook(tgt);
        }
    }
}

void hookC(){
	const char *libName = "libc.so";

	const char *procName = "com.jingdong.app.mall";

	LOGD("init");
	lib_addr = get_lib_base(procName,libName);
	LOGD("libaddr:%x", lib_addr);


	if( (int)lib_addr != 0 && (int)lib_addr != -1 ){
		LOGD("hooked lib:%s, proc:%s, base addr:%p", libName, procName, lib_addr);
		Item funcItems0[] = {
				{ 0x00025110, pre_usleep, NULL, true},
//				{ 0x00014688, pre_strlen, NULL, true},
		};

		for(int i = 0; i < sizeof(funcItems0)/sizeof(funcItems0[0]) ; i ++){
			zpointer tgt = lib_addr + funcItems0[i].addr + ( funcItems0[i].isThumb ? 1 : 0 );
			ZzBuildHook(tgt, NULL, NULL, funcItems0[i].pre_call_ptr, funcItems0[i].post_call_ptr, false);
			ZzEnableHook(tgt);
		}
	}
}

void hookJd(){
	const char *libName = "libjdbitmapkit.so";

	const char *procName = "com.jingdong.app.mall";

	LOGD("init");
	lib_addr = get_lib_base(procName,libName);
	LOGD("libaddr:%x", lib_addr);


	if( (int)lib_addr != 0 && (int)lib_addr != -1 ){
		LOGD("hooked lib:%s, proc:%s, base addr:%p", libName, procName, lib_addr);
		Item funcItems0[] = {
				{ 0x00002368, pre_md4, aft_md4, true},
		};

		for(int i = 0; i < sizeof(funcItems0)/sizeof(funcItems0[0]) ; i ++){
			zpointer tgt = lib_addr + funcItems0[i].addr + ( funcItems0[i].isThumb ? 1 : 0 );
			ZzBuildHook(tgt, NULL, NULL, funcItems0[i].pre_call_ptr, funcItems0[i].post_call_ptr, false);
			ZzEnableHook(tgt);
		}
	}
}

__attribute__((constructor)) void hook(){
//	hookC();
	hookArt();
//	hookJd();
}
