//
// Created by mbx on 2017/12/22.
//

#include <HookZz/include/hookzz.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <android/log.h>

#include "utils.h"

#define LOG_TAG "Xposed:Inject:so"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__);


const char *libName = "libezstreamclient.so";
const char *procName = "com.videogo";

FILE *fv = NULL;
FILE *fa = NULL;

typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;

void pre_create_message(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_create_message");

    LOGD("r0:%#x",rs->general.regs.r0);

    uint32_t r1 = rs->general.regs.r1;
    STACK_SET(callstack, "r1", r1 , uint32_t *);

    LOGD("r2:%#x,r3:%#x",rs->general.regs.r2,rs->general.regs.r3);

}
void aft_create_message(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------aft_create_message");
    uint32_t r1;
    if (STACK_CHECK_KEY(callstack, "r1")) {
        r1 = STACK_GET(callstack, "r1", uint32_t );
    }
    char buf[200] = {'\0'};
    ByteAdapterToStr( * (_DWORD *) r1,buf,50);
    LOGD("r1:%s",buf);
}


void pre_parse_msg_header(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_parse_msg_header");
    //char buf[300] = {'\0'};
    //ByteAdapterToStr(  rs->general.regs.r0,buf,50);
    //LOGD("r0:%s",buf);
    //LOGD("lr:%#x",rs->lr);

    //CStreamCln this
    uint32_t this = (uint32_t *) rs->general.regs.r0 - 20;
    STACK_SET(callstack, "this", this , uint32_t *);
}
void aft_parse_msg_header(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------aft_parse_msg_header");

    uint32_t *this;
    if (STACK_CHECK_KEY(callstack, "this")) {
        this = STACK_GET(callstack, "this", uint32_t *);
    }
    LOGD("switch:%d",*(this + 4148));

}


hookPlay(const char * procName,const char * libName);
hookSplite(const char * procName,const char * libName);
__attribute__((constructor)) void hook(){

    //hookPlay(procName ,"libPlayCtrl_L.so");

    hookSplite(procName ,"libPlayCtrl_L.so");

    uint32_t lib_addr = get_lib_base(procName, libName);
    LOGD("hooked lib:%s, base addr:%p", libName, lib_addr);

    uint32_t create_message = 0x115bdc;
    zpointer tgt_create_message = lib_addr + (uint32_t) 1 + create_message;
    ZzBuildHook(tgt_create_message, NULL, NULL, pre_create_message, aft_create_message, false);
    ZzEnableHook(tgt_create_message);

    // ParseStreamMsgHead
    uint32_t parse_msg_header = 0x122af8;
    zpointer tgt_parse_msg_header = lib_addr + (uint32_t) 1 + parse_msg_header;
    ZzBuildHook(tgt_parse_msg_header, NULL, NULL, pre_parse_msg_header, aft_parse_msg_header, false);
    //ZzEnableHook(tgt_parse_msg_header);

    //ProcessNetworkMsg

}

hookSplite(const char * procName,const char * libName) {
    uint32_t lib_addr = get_lib_base(procName, libName);
    LOGD("hooked lib:%s, base addr:%p", libName, lib_addr);

    //hik_rtp_process_payload
}



















































/**
void pre_DecodeFrame(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_CHikIntelDec::DecodeFrameDecodeFrame");
    LOGD("lr:%#x",rs->lr);
    LOGD("*a4:%#x",* (_DWORD*) rs->general.regs.r3);
    char *buf[200] = {'\0'};
    ByteAdapterToStr(rs->general.regs.r1,buf,30);
    LOGD("src:%s",buf);
    LOGD("lr:%#x",rs->lr);
}

void pre_CHKVDecoder(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_CHK V Decoder");
    char *buf[200] = {'\0'};
    ByteAdapterToStr(rs->general.regs.r1,buf,40);
    LOGD("a2:%s",buf);
    LOGD("len:%#x",rs->general.regs.r2);
    LOGD("lr:%#x",rs->lr);

    //size_t wirte_size = fwrite(rs->general.regs.r1, 1 ,rs->general.regs.r2,fv);
    //LOGD("wirte:%#x",wirte_size);
}


void pre_CHKADecoder(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_CHK A Decoder");
    char *buf[200] = {'\0'};
    ByteAdapterToStr(rs->general.regs.r1,buf,40);
    LOGD("a2:%s",buf);
    LOGD("len:%#x",rs->general.regs.r2);
    LOGD("lr:%#x",rs->lr);

    //size_t wirte_size = fwrite(rs->general.regs.r1, 1 ,rs->general.regs.r2,fa);
    //LOGD("wirte:%#x",wirte_size);
}
void pre_OutputFrameDataEv(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_OutputFrameDataEv");
    LOGD("lr:%#x",rs->lr);

}
void pre_CMPEG2Splitter15OutputFrameDataEv(RegState *rs, ThreadStack *threadstack, CallStack *callstack) {
    LOGD("hooked so pre call -----------pre_CMPEG2Splitter15OutputFrameDataEv");
    LOGD("lr:%#x",rs->lr);

}




hookPlay(const char * procName,const char * libName){


    fv = fopen("/data/local/tmp/video.dat","wb+");
    if( fv == NULL){
        LOGD("open file error ..........");
    }
    fa = fopen("/data/local/tmp/audio.dat","wb+");
    if( fa == NULL){
        LOGD("open file error ..........");
    }

    uint32_t lib_addr = get_lib_base(procName, libName);
    LOGD("hooked lib:%s, base addr:%p", libName, lib_addr);

    //CHikIntelDec::DecodeFrameDecodeFrame
    uint32_t DecodeFrame = 0xa1448;
    zpointer tgt_DecodeFrame = lib_addr + (uint32_t) 1 + DecodeFrame;
    ZzBuildHook(tgt_DecodeFrame, NULL, NULL, pre_DecodeFrame, NULL, false);
    //ZzEnableHook(tgt_DecodeFrame);

    //CDecoder::SetDecodeEngine
    //uint32_t SetDecodeEngine = 0x8d79c;

    //H264D_THREAD_Process H264D_THREAD_FrameExecute   H264D_THREAD_FrameSubmit  H264D_decode_poc
    //AVC_DecodeOneFrame
    //uint32_t AVC_DecodeOneFrame = 0x1dc494;

    //CAVC264Decoder::DecodeOneFrame
    //uint32_t AVC_DecodeOneFrame = 0x13b338;


    //CHKVDecoder::VDecodeFrame
    //uint32_t AVC_DecodeOneFrame = 0x9c9c0;

    // CHKVDecoder::DecodeFrame
    uint32_t CHKVDecoder = 0x9be2c;
    zpointer tgt_CHKVDecoder = lib_addr + (uint32_t) 1 + CHKVDecoder;
    ZzBuildHook(tgt_CHKVDecoder, NULL, NULL, pre_CHKVDecoder, NULL, false);
    ZzEnableHook(tgt_CHKVDecoder);

    //ZN13CStreamSource15SendMediaHeaderEP11CBaseOutput
    //HIK_AACDEC_Decode
    //PSDK_CAACDecoder::DecodeOneFrame
    // CHKADecoder::ADecodeFrame
    //CHKADecoder::DecodeFrame

    uint32_t CHKADecoder = 0x9abf8;
    zpointer tgt_CHKADecoder = lib_addr + (uint32_t) 1 + CHKADecoder;
    ZzBuildHook(tgt_CHKADecoder, NULL, NULL, pre_CHKADecoder, NULL, false);
    ZzEnableHook(tgt_CHKADecoder);

    uint32_t OutputFrameDataEv = 0xb67a4;
    zpointer tgt_OutputFrameDataEv = lib_addr + (uint32_t) 1 + OutputFrameDataEv;
    ZzBuildHook(tgt_OutputFrameDataEv, NULL, NULL, pre_OutputFrameDataEv, NULL, false);
    ZzEnableHook(tgt_OutputFrameDataEv);

    // CMPEG2Splitter::OutputFrameData
    uint32_t CMPEG2Splitter15OutputFrameDataEv = 0xb7640;
    zpointer tgt_CMPEG2Splitter15OutputFrameDataEv = lib_addr + (uint32_t) 1 + CMPEG2Splitter15OutputFrameDataEv;
    ZzBuildHook(tgt_CMPEG2Splitter15OutputFrameDataEv, NULL, NULL, pre_CMPEG2Splitter15OutputFrameDataEv, NULL, false);
    ZzEnableHook(tgt_CMPEG2Splitter15OutputFrameDataEv);

}
*/


