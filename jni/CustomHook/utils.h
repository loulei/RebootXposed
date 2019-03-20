//
// Created by mbx on 2017/12/12.
//

#ifndef XPINJECT_UTILS_H
#define XPINJECT_UTILS_H


typedef unsigned int uint32_t;

void getPid();
int getCallStack();

uint32_t get_lib_base(const char* proc_name,const char* lib_name);

//十六进制字符串转换为字节流
//void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

void ByteAdapterToStr(const char* source, char* dest, int sourceLen);

//字节流转换为十六进制字符串
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);



#endif //XPINJECT_UTILS_H

