#pragma once

#include<zlib-1.2.11/zlib.h>

#include<zlib-1.2.11/zconf.h>

#include <iostream>

using namespace std;
class ZipTool
{
public:
	static unsigned char* zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen);//压缩,传入待压缩数组，待压缩数组的大小，用于储存压缩后数组长度的变量
	static unsigned char* zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen,int level);//分等级压缩,0-9,9为极限压缩
	static unsigned char* unzip(unsigned char* src, unsigned long srcLen, unsigned long* destLen);//解压，传入待解压数组，待解压数组长度，未压缩时的长度
};

