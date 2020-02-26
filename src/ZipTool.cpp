#include "ZipTool.h"
#include"parse.h"
#include<cstring>
#define MAX_BUFFER 1024*1024*10

/*
接口说明:
zip函数有两个，分别是普通压缩和可以选等级的压缩算法
unzip函数有一个，为解压算法
zip函数需要传入待压缩数组,待压缩数组长度，压缩后长度的指针，以及压缩等级*(可选)
unzip函数传入待解压数组，待解压长度，解压后的长度
*/
unsigned char*  ZipTool::zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen)
{
	//unsigned char* expArray = parseArray(src, srcLen);//获取扩增后的数组

	//long expLen = srcLen * 8;//扩增后数组的长度

	unsigned char* tmp = new unsigned char[srcLen];//压缩后的储存空间，暂时申请为与src同等大小
	
	*destLen = srcLen;//初始化压缩后的长度和原始长度相同
	compress(tmp, destLen, src, srcLen);
	unsigned char* dest = new unsigned char[*destLen];//用于储存压缩后的数据
	memcpy(dest, tmp, *destLen);
	delete []tmp;
	//delete[]expArray;
	cout << "压缩成功 " << "压缩率为" << (*destLen /((1.0)* srcLen))<<endl;
	return dest;
}

unsigned char* ZipTool::zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen,int level)
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


