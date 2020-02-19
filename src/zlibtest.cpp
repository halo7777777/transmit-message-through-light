#define _CRT_SECURE_NO_WARNINGS

#include"ZipTool.h"
#include<ctime>
int main()
{
	unsigned char* src = new unsigned char[1024];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 1024; i++)
	{
		src[i] = rand()%127;
	}
	unsigned long dstLen = 0;
	unsigned long srcLen = 1024;
	unsigned char* dst = ZipTool::zip(src, srcLen, &dstLen);
	unsigned long unzipLen = 1024;
	cout << dstLen<<endl;
	unsigned char* unzipdest = ZipTool::unzip(dst, dstLen, &unzipLen);
	cout << unzipLen<<endl;
	for (int i = 0; i < 10; i++)
	{
		printf_s("%d %d\n", src[i], unzipdest[i]);
	}
	return 0;
}