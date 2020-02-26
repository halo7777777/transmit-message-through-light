//#include<iostream>
//#include<ctime>
//#include"parse.h"
//using namespace std;
//int main()
//{
//	unsigned char* src = new unsigned char[1024];
//	srand((unsigned)time(NULL));
//	for (int i = 0; i < 1024; i++)
//	{
//		src[i] = rand() % 127;
//	}
//	unsigned char* tmp = parseArray(src, 1024);
//	int parselen = 1024 * 8;
//	for (int i = 0; i < 10; i++)
//	{
//		printf_s("%d ", tmp[i]);
//	}
//	unsigned char* dst = inverseParse(tmp, parselen);
//	for (int i = 0; i < 10; i++)
//	{
//		printf_s("%d %d\n", src[i], dst[i]);
//	}
//	delete[]src;
//	delete[]tmp;
//	delete[]dst;
//	return 0;
//}