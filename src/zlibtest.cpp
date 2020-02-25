//#define _CRT_SECURE_NO_WARNINGS
//
//#include"ZipTool.h"
//#include<ctime>
//
///*测试结论:
//10M的文件，使用ASCII码写入时，使用ZLIB算法，可以有百分之87的压缩率
//而使用7z算法时，可以有0.58的压缩率*/
//int main()
//{
//	unsigned char* src = new unsigned char[1024*1024*10];
//	srand((unsigned)time(NULL));
//	for (int i = 0; i < 1024 * 1024 * 10; i++)
//	{
//		src[i] = rand()%128;
//	}
//	unsigned long dstLen = 0;
//	unsigned long srcLen = 1024 * 1024 * 10;
//	unsigned char* dst = ZipTool::zip(src, srcLen, &dstLen);
//	cout <<dstLen<<endl;
//	//unsigned long unzipLen = 1024;
//	//cout << dstLen<<endl;
//	//unsigned char* unzipdest = ZipTool::unzip(dst, dstLen, &unzipLen);
//	//cout << unzipLen<<endl;
//	//for (int i = 0; i < 10; i++)
//	//{
//	//	printf_s("%d %d\n", src[i], unzipdest[i]);
//	//}
//	//return 0;
//}