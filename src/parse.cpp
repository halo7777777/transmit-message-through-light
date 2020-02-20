#include"parse.h"
void dectobin(unsigned char num, vector<unsigned char>& v)
{
	if (num / 2 != 0)
	{
		dectobin((unsigned char)(num / 2),v);
	}
	v.push_back(num % 2);
}

unsigned char* parseArray(unsigned char* src, long srcLen)//将字符数组转换为01表示，扩大8倍
{
	unsigned char* dst = new unsigned char[srcLen *8];
	int i = 0;
	for (int j = 0; j <srcLen; j++)
	{
		vector<unsigned char> v;
		dectobin(src[j], v);
		if (v.size() != 8)
		{
			for (int k=0;k<8-v.size();k++)
			{
				dst[i++] = 0;
			}
		}
		for (int k = 0; k < v.size(); k++)
		{
			dst[i++] = v[k];
		}
	}
	return dst;
}

unsigned char* inverseParse(unsigned char* src, long srcLen)
{
	unsigned char* dst = new unsigned char[srcLen / 8];
	int index = 0;
	for (int i = 0; i < srcLen; i += 8)//外循环遍历01数组
	{
		int k = 0;
		int ind = 1;
		for (int j = 7; j >= 0 && i + j < srcLen; j--)//将7位二进制转换为10进制
		{
			k += (src[i + j] * ind);
			ind *= 2;
		}
		dst[index++] = (unsigned char)k;
	}
	return dst;
}