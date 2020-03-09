#include<iostream>
#include"Decode.h"
#include"FileConvert.h"
#include<cstdio>
#include<cstdlib>
#include"code.h"
#include"qtdecode.h"
using namespace std;
using namespace cv;

int main()
{
	int fps = 10;
	FileConvert converter;
	
	string path = "imageSrc\\";
	int function;
	cout << "function one:encode file to video" << endl;
	cout << "function two:decode video to file" << endl;
	cout << "Calculate bit error rate" << endl;
	cout << "input function you choose" << endl;
	cin >> function;
	if (function == 1)
	{
		//选择输入字符串or生成随机比特流//根据字符串生成文件的接口还没有写
		unsigned char* bytes = NULL;
		unsigned long size;
		cout << "Please input size :";
		char filename[] = "in.bin";
		cin >> size;//需要生成文件的大小，单位字节

		converter.GenerateRandFile(filename, size);
		bytes = converter.FileToByte(filename, &size);//读取文件生成的Byte数组
		
		Code::Main(bytes, size, "imageSrc\\", "png");
		converter.PicTransToVideo(fps);
		//int fps = 10;//测试用每秒10帧试试
		
		return 0;
	}
	//
	//	用手机录制视频并拍摄

	else if (function == 2)
	{
		qtdecode mydecode;
		mydecode.mydecode();
	}
	else if (function == 3)
	{
		FileConvert file;
		FILE* fp1, * fp2;
		unsigned long num1 = 0, num2 = 0;
		fopen_s(&fp1, "in.bin", "rb");
		fopen_s(&fp2, "out.bin", "rb");
		num1 = file.GetFileSize(fp1);
		num2 = file.GetFileSize(fp1);
		unsigned char* dst1 = new unsigned char[num1];
		unsigned char* dst2 = new unsigned char[num2];
		fread(dst1, sizeof(unsigned char), num1, fp1);
		fread(dst2, sizeof(unsigned char), num1, fp2);
		unsigned long totalnum = 1, wrongnum = 0, m = 0;
		while (totalnum <= (num1 + 1))
		{
			totalnum++;
			if (dst1[m] != dst2[m]) wrongnum++;
			m++;
		}
		double n;
		n = 1 - (wrongnum / totalnum);
		cout << "bit error rate is " << n << endl;
		return 0;
	}
}