#include "ZipTool.h"
#include<cstring>
#define MAX_BUFFER 1024*1024*10
unsigned char*  ZipTool::zip(const unsigned char* src, unsigned long srcLen, unsigned long* destLen)
{
	unsigned char* tmp = new unsigned char[srcLen];//压缩后的储存空间，暂时申请为与src同等大小
	*destLen = srcLen;//初始化压缩后的长度和原始长度相同
	compress(tmp, destLen, src, srcLen);
	unsigned char* dest = new unsigned char[*destLen];//用于储存压缩后的数据
	memcpy(dest, tmp, *destLen);
	delete []tmp;
	cout << "压缩成功 " << "压缩率为" << (*destLen /((1.0)* srcLen))<<endl;
	return dest;
}

unsigned char* ZipTool::zip(const unsigned char* src, unsigned long srcLen, unsigned long* destLen,int level)
{
	unsigned char* tmp = new unsigned char[srcLen];
	*destLen = srcLen;
	compress2(tmp, destLen, src, srcLen,level);
	unsigned char* dest = new unsigned char[*destLen];
	memcpy(dest, tmp, *destLen);
	delete[]tmp;
	cout << "压缩成功 " << "压缩率为" << (*destLen / ((1.0) * srcLen)) <<endl;
	return dest;
}

unsigned char* ZipTool::unzip(unsigned char* src, unsigned long srcLen, unsigned long* destLen)
{
	unsigned char* tmp = new unsigned char[MAX_BUFFER];//暂存解压后的数组
	uncompress(tmp, destLen, src, srcLen);
	//cout << *destLen << "--解压后" << endl;
	unsigned char* dest = new unsigned char[*destLen];
	memcpy(dest, tmp, *destLen);
	delete[]tmp;
	cout << "解压成功 " << "解压后长度为" << *destLen << endl;
	return dest;
}


