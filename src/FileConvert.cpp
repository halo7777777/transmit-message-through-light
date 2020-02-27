#define _CRT_SECURE_NO_WARNINGS
#include "FileConvert.h"
#include<iostream>
#include<ctime>
#include<cstdio>


using namespace std;


FileConvert::~FileConvert()
{
	delete[]dst;
}

unsigned long FileConvert::GetFileSize(FILE* p)
{
	unsigned int current_read_position = ftell(p);
	fseek(p, 0, SEEK_END);
	unsigned long size = ftell(p);
	fseek(p, current_read_position, SEEK_SET);
	return size;
}

unsigned char* FileConvert::FileToByte(char filename[], unsigned long* size)
{
	p = fopen(filename, "rb");
	*size = GetFileSize(p);
	unsigned char* dst = new unsigned char[*size];
	fread(dst, sizeof(unsigned char), *size, p);
	fclose(p);
	return dst;
}//读取文件生成数组

void FileConvert::GenerateRandomArray(unsigned char* src, unsigned long size)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < size; i++)
	{
		src[i] = rand();
	}
}//根据指定大小生成随机数组

void FileConvert::ByteToFile(unsigned char* src, char filename[], unsigned long size)
{
	p = fopen(filename, "wb");
	fwrite(src, sizeof(unsigned char), size, p);
	fclose(p);
}//将数据写入文件中

void FileConvert::GenerateRandFile(char filename[], unsigned long size)
{
	unsigned char* tmp = new unsigned char[size];
	GenerateRandomArray(tmp, size);
	ByteToFile(tmp, filename, size);
	delete[]tmp;
}//生成随机文件

void FileConvert::PicTransToVideo(int picNumPersec)//每秒帧数
{
	string firstSetting, secondSetting, str3;
	string outputname;
	char picnum[5], command[200];
	sprintf_s(picnum, "%d", picNumPersec);
	cout << "please input the output video name:" << endl;
	cin >> outputname;
	firstSetting = "ffmpeg -f image2 -r ";
	secondSetting = " -i imageSrc\\%d.png -vcodec mpeg4 videoOutput\\";
	str3 = firstSetting + picnum + secondSetting + outputname;
	sprintf_s(command, "%s", str3.c_str());
	system(command);
}// 命令格式 ffmpeg - f image2 - r fps - i imageSrc\%d.jpg - vcodec mpeg4 output\\%s.mp4 将图片保存至output目录下

void FileConvert::VideoTransToPic()
{
	string firstSetting, formatName, videoName,str3;
	char command[200];
	firstSetting = "ffmpeg -i videoSrc\\";
	formatName = " imageOutput\\%d.png";
	cout << "Please enter video name:" << endl;
	cin >> videoName;
	str3 = firstSetting + videoName + formatName; //组合生成ffmpeg命令
	sprintf_s(command, "%s", str3.c_str()); //转化类型
	system(command);
}
